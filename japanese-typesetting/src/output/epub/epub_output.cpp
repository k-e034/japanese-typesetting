/**
 * @file epub_output.cpp
 * @brief EPUB出力エンジンの詳細実装
 */

#include "japanese_typesetting/output/epub_output.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <zip.h>

namespace japanese_typesetting {
namespace output {

EpubOutputEngine::EpubOutputEngine() {
    // 特に初期化処理はない
}

EpubOutputEngine::~EpubOutputEngine() {
    // 特に何もしない
}

bool EpubOutputEngine::outputEpub(
    const core::document::Document& document,
    const core::style::Style& style,
    const std::string& outputPath,
    const EpubOutputOptions& options) {
    
    try {
        // HTMLコンテンツの生成
        std::map<std::string, std::string> contentFiles = generateHtmlContent(document, style, options);
        
        // EPUBファイルの生成
        return generateEpub(contentFiles, outputPath, options);
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "EPUB出力中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

bool EpubOutputEngine::generateEpub(
    const std::map<std::string, std::string>& contentFiles,
    const std::string& outputPath,
    const EpubOutputOptions& options) {
    
    // 一時ディレクトリの作成
    std::string tempDir = createTempDir();
    
    try {
        // MIMETYPEファイルの作成
        std::string mimetypePath = tempDir + "/mimetype";
        std::ofstream mimetypeFile(mimetypePath);
        mimetypeFile << "application/epub+zip";
        mimetypeFile.close();
        
        // META-INFディレクトリの作成
        std::filesystem::create_directory(tempDir + "/META-INF");
        
        // container.xmlの作成
        std::string containerPath = tempDir + "/META-INF/container.xml";
        std::ofstream containerFile(containerPath);
        containerFile << generateContainer();
        containerFile.close();
        
        // OEBPSディレクトリの作成
        std::filesystem::create_directory(tempDir + "/OEBPS");
        std::filesystem::create_directory(tempDir + "/OEBPS/css");
        std::filesystem::create_directory(tempDir + "/OEBPS/images");
        std::filesystem::create_directory(tempDir + "/OEBPS/fonts");
        
        // CSSファイルの作成
        std::string cssPath = tempDir + "/OEBPS/css/style.css";
        std::ofstream cssFile(cssPath);
        cssFile << generateCss(core::style::Style(), options);
        cssFile.close();
        
        // コンテンツファイルの作成
        std::vector<std::string> contentFileNames;
        for (const auto& [path, content] : contentFiles) {
            std::string fullPath = tempDir + "/OEBPS/" + path;
            
            // ディレクトリの作成（必要な場合）
            std::filesystem::path dirPath = std::filesystem::path(fullPath).parent_path();
            std::filesystem::create_directories(dirPath);
            
            // ファイルの作成
            std::ofstream file(fullPath);
            file << content;
            file.close();
            
            contentFileNames.push_back(path);
        }
        
        // 表紙画像のコピー（存在する場合）
        if (!options.coverImagePath.empty() && std::filesystem::exists(options.coverImagePath)) {
            std::string destPath = tempDir + "/OEBPS/images/cover.jpg";
            std::filesystem::copy_file(options.coverImagePath, destPath, std::filesystem::copy_options::overwrite_existing);
            contentFileNames.push_back("images/cover.jpg");
        }
        
        // OPFファイルの作成
        std::string opfPath = tempDir + "/OEBPS/content.opf";
        std::ofstream opfFile(opfPath);
        opfFile << generateOpf(contentFileNames, options);
        opfFile.close();
        
        // NCXファイルの作成
        std::string ncxPath = tempDir + "/OEBPS/toc.ncx";
        std::ofstream ncxFile(ncxPath);
        ncxFile << generateNcx(core::document::Document(), options);
        ncxFile.close();
        
        // EPUBファイルの作成（ZIPアーカイブ）
        int err = 0;
        zip_t* zip = zip_open(outputPath.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
        if (zip == nullptr) {
            throw std::runtime_error("EPUBファイルの作成に失敗しました");
        }
        
        // mimetypeを最初に追加（非圧縮）
        zip_source_t* mimetypeSource = zip_source_file(zip, mimetypePath.c_str(), 0, 0);
        zip_int64_t index = zip_file_add(zip, "mimetype", mimetypeSource, ZIP_FL_ENC_GUESS);
        zip_set_file_compression(zip, index, ZIP_CM_STORE, 0);
        
        // その他のファイルを追加（圧縮）
        auto addFileToZip = [&](const std::string& path, const std::string& zipPath) {
            zip_source_t* source = zip_source_file(zip, path.c_str(), 0, 0);
            zip_file_add(zip, zipPath.c_str(), source, ZIP_FL_ENC_GUESS);
        };
        
        // META-INFディレクトリの追加
        addFileToZip(containerPath, "META-INF/container.xml");
        
        // OEBPSディレクトリの追加
        addFileToZip(opfPath, "OEBPS/content.opf");
        addFileToZip(ncxPath, "OEBPS/toc.ncx");
        addFileToZip(cssPath, "OEBPS/css/style.css");
        
        // コンテンツファイルの追加
        for (const auto& [path, _] : contentFiles) {
            addFileToZip(tempDir + "/OEBPS/" + path, "OEBPS/" + path);
        }
        
        // 表紙画像の追加（存在する場合）
        if (!options.coverImagePath.empty() && std::filesystem::exists(options.coverImagePath)) {
            addFileToZip(tempDir + "/OEBPS/images/cover.jpg", "OEBPS/images/cover.jpg");
        }
        
        // ZIPアーカイブのクローズ
        zip_close(zip);
        
        // 一時ディレクトリの削除
        removeTempDir(tempDir);
        
        return true;
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "EPUBファイルの生成中にエラーが発生しました: " << e.what() << std::endl;
        
        // 一時ディレクトリの削除
        removeTempDir(tempDir);
        
        return false;
    }
}

std::map<std::string, std::string> EpubOutputEngine::generateHtmlContent(
    const core::document::Document& document,
    const core::style::Style& style,
    const EpubOutputOptions& options) {
    
    std::map<std::string, std::string> contentFiles;
    
    // 文書の組版
    double contentWidth = 800.0; // 仮の幅
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(document, style, contentWidth);
    
    // チャプターごとにHTMLファイルを生成
    int chapterCount = 1;
    std::vector<core::typesetting::TextBlock> chapterBlocks;
    
    for (const auto& block : blocks) {
        chapterBlocks.push_back(block);
        
        // 一定数のブロックごとにチャプターを区切る（実際には文書構造に基づいて区切る）
        if (chapterBlocks.size() >= 10 || &block == &blocks.back()) {
            // HTMLの生成
            std::ostringstream html;
            
            html << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                 << "<!DOCTYPE html>\n"
                 << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"ja\">\n"
                 << "<head>\n"
                 << "  <meta charset=\"UTF-8\" />\n"
                 << "  <title>" << options.title << " - Chapter " << chapterCount << "</title>\n"
                 << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />\n"
                 << "</head>\n"
                 << "<body class=\"" << (options.vertical ? "vertical" : "horizontal") << "\">\n";
            
            // チャプタータイトル
            html << "  <h1>Chapter " << chapterCount << "</h1>\n";
            
            // 文書内容の変換
            html << blocksToHtml(chapterBlocks, options);
            
            html << "</body>\n"
                 << "</html>\n";
            
            // ファイル名の生成
            std::ostringstream fileName;
            fileName << "chapter" << std::setw(3) << std::setfill('0') << chapterCount << ".xhtml";
            
            // コンテンツファイルに追加
            contentFiles[fileName.str()] = html.str();
            
            // チャプターカウントの更新
            chapterCount++;
            chapterBlocks.clear();
        }
    }
    
    // 表紙ページの生成
    std::ostringstream coverHtml;
    coverHtml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
              << "<!DOCTYPE html>\n"
              << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"ja\">\n"
              << "<head>\n"
              << "  <meta charset=\"UTF-8\" />\n"
              << "  <title>" << options.title << "</title>\n"
              << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />\n"
              << "</head>\n"
              << "<body>\n"
              << "  <div class=\"cover\">\n";
    
    // 表紙画像（存在する場合）
    if (!options.coverImagePath.empty()) {
        coverHtml << "    <img src=\"images/cover.jpg\" alt=\"Cover\" />\n";
    }
    
    coverHtml << "    <h1>" << options.title << "</h1>\n"
              << "    <p class=\"author\">" << options.author << "</p>\n"
              << "  </div>\n"
              << "</body>\n"
              << "</html>\n";
    
    contentFiles["cover.xhtml"] = coverHtml.str();
    
    // 目次ページの生成
    std::ostringstream tocHtml;
    tocHtml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            << "<!DOCTYPE html>\n"
            << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\" xml:lang=\"ja\">\n"
            << "<head>\n"
            << "  <meta charset=\"UTF-8\" />\n"
            << "  <title>" << options.title << " - 目次</title>\n"
            << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"css/style.css\" />\n"
            << "</head>\n"
            << "<body>\n"
            << "  <h1>目次</h1>\n"
            << "  <nav epub:type=\"toc\" id=\"toc\">\n"
            << "    <ol>\n";
    
    // チャプターへのリンク
    for (int i = 1; i < chapterCount; i++) {
        std::ostringstream fileName;
        fileName << "chapter" << std::setw(3) << std::setfill('0') << i << ".xhtml";
        
        tocHtml << "      <li><a href=\"" << fileName.str() << "\">Chapter " << i << "</a></li>\n";
    }
    
    tocHtml << "    </ol>\n"
            << "  </nav>\n"
            << "</body>\n"
            << "</html>\n";
    
    contentFiles["toc.xhtml"] = tocHtml.str();
    
    return contentFiles;
}

std::string EpubOutputEngine::generateCss(
    const core::style::Style& style,
    const EpubOutputOptions& options) {
    
    // CSSの生成
    std::ostringstream css;
    
    // 基本スタイル
    css << "@charset \"UTF-8\";\n\n"
        << "html, body {\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "  font-size: " << options.fontSize << "pt;\n"
        << "  font-family: \"" << options.fontFamily << "\", sans-serif;\n"
        << "  line-height: " << options.lineHeight << ";\n"
        << "}\n\n";
    
    // 縦書きモード
    css << "body.vertical {\n"
        << "  writing-mode: vertical-rl;\n"
        << "  -webkit-writing-mode: vertical-rl;\n"
        << "  -epub-writing-mode: vertical-rl;\n"
        << "  text-orientation: upright;\n"
        << "}\n\n";
    
    // 横書きモード
    css << "body.horizontal {\n"
        << "  writing-mode: horizontal-tb;\n"
        << "  -webkit-writing-mode: horizontal-tb;\n"
        << "  -epub-writing-mode: horizontal-tb;\n"
        << "}\n\n";
    
    // 見出しスタイル
    css << "h1 {\n"
        << "  font-size: 1.5em;\n"
        << "  font-weight: bold;\n"
        << "  margin: 1em 0;\n"
        << "}\n\n"
        << "h2 {\n"
        << "  font-size: 1.3em;\n"
        << "  font-weight: bold;\n"
        << "  margin: 0.8em 0;\n"
        << "}\n\n"
        << "h3 {\n"
        << "  font-size: 1.1em;\n"
        << "  font-weight: bold;\n"
        << "  margin: 0.6em 0;\n"
        << "}\n\n";
    
    // 段落スタイル
    css << "p {\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "  text-align: justify;\n"
        << "  text-indent: 1em;\n"
        << "}\n\n";
    
    // 表紙スタイル
    css << ".cover {\n"
        << "  text-align: center;\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "}\n\n"
        << ".cover img {\n"
        << "  max-width: 100%;\n"
        << "  max-height: 100%;\n"
        << "}\n\n"
        << ".cover h1 {\n"
        << "  font-size: 2em;\n"
        << "  margin: 1em 0 0.5em 0;\n"
        << "}\n\n"
        << ".cover .author {\n"
        << "  font-size: 1.2em;\n"
        << "  margin: 0.5em 0 1em 0;\n"
        << "}\n\n";
    
    // 目次スタイル
    css << "nav#toc ol {\n"
        << "  list-style-type: none;\n"
        << "  margin: 1em 0;\n"
        << "  padding: 0;\n"
        << "}\n\n"
        << "nav#toc li {\n"
        << "  margin: 0.5em 0;\n"
        << "}\n\n"
        << "nav#toc a {\n"
        << "  text-decoration: none;\n"
        << "  color: #000;\n"
        << "}\n\n";
    
    // ルビスタイル
    css << "ruby {\n"
        << "  ruby-align: center;\n"
        << "}\n\n";
    
    return css.str();
}

std::string EpubOutputEngine::generateOpf(
    const std::vector<std::string>& contentFiles,
    const EpubOutputOptions& options) {
    
    // 現在時刻の取得
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    
    std::ostringstream dateStream;
    dateStream << std::put_time(tm, "%Y-%m-%dT%H:%M:%SZ");
    std::string dateString = dateStream.str();
    
    // OPFファイルの生成
    std::ostringstream opf;
    
    opf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"3.0\" unique-identifier=\"BookId\">\n"
        << "  <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n"
        << "    <dc:identifier id=\"BookId\">" << options.identifier << "</dc:identifier>\n"
        << "    <dc:title>" << options.title << "</dc:title>\n"
        << "    <dc:language>" << options.language << "</dc:language>\n"
        << "    <dc:creator>" << options.author << "</dc:creator>\n"
        << "    <dc:publisher>" << options.publisher << "</dc:publisher>\n"
        << "    <dc:description>" << options.description << "</dc:description>\n"
        << "    <dc:rights>" << options.rights << "</dc:rights>\n"
        << "    <dc:date>" << dateString << "</dc:date>\n"
        << "    <meta property=\"dcterms:modified\">" << dateString << "</meta>\n";
    
    // 表紙画像の指定（存在する場合）
    if (!options.coverImagePath.empty()) {
        opf << "    <meta name=\"cover\" content=\"cover-image\" />\n";
    }
    
    // その他のメタデータ
    for (const auto& [name, content] : options.extraMetadata) {
        opf << "    <meta name=\"" << name << "\" content=\"" << content << "\" />\n";
    }
    
    opf << "  </metadata>\n"
        << "  <manifest>\n";
    
    // アイテムの追加
    opf << "    <item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />\n"
        << "    <item id=\"css\" href=\"css/style.css\" media-type=\"text/css\" />\n"
        << "    <item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\" />\n"
        << "    <item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\" properties=\"nav\" />\n";
    
    // 表紙画像の追加（存在する場合）
    if (!options.coverImagePath.empty()) {
        opf << "    <item id=\"cover-image\" href=\"images/cover.jpg\" media-type=\"image/jpeg\" properties=\"cover-image\" />\n";
    }
    
    // コンテンツファイルの追加
    int itemCount = 1;
    for (const auto& path : contentFiles) {
        if (path == "cover.xhtml" || path == "toc.xhtml" || path == "images/cover.jpg" || path == "css/style.css") {
            continue; // 既に追加済み
        }
        
        std::string mediaType;
        if (path.ends_with(".xhtml")) {
            mediaType = "application/xhtml+xml";
        } else if (path.ends_with(".jpg") || path.ends_with(".jpeg")) {
            mediaType = "image/jpeg";
        } else if (path.ends_with(".png")) {
            mediaType = "image/png";
        } else if (path.ends_with(".gif")) {
            mediaType = "image/gif";
        } else if (path.ends_with(".svg")) {
            mediaType = "image/svg+xml";
        } else if (path.ends_with(".css")) {
            mediaType = "text/css";
        } else if (path.ends_with(".js")) {
            mediaType = "application/javascript";
        } else if (path.ends_with(".ttf")) {
            mediaType = "application/font-sfnt";
        } else if (path.ends_with(".otf")) {
            mediaType = "application/font-sfnt";
        } else if (path.ends_with(".woff")) {
            mediaType = "application/font-woff";
        } else {
            mediaType = "application/octet-stream";
        }
        
        opf << "    <item id=\"item" << itemCount << "\" href=\"" << path << "\" media-type=\"" << mediaType << "\" />\n";
        itemCount++;
    }
    
    opf << "  </manifest>\n"
        << "  <spine toc=\"ncx\">\n"
        << "    <itemref idref=\"cover\" />\n"
        << "    <itemref idref=\"toc\" />\n";
    
    // チャプターの追加
    itemCount = 1;
    for (const auto& path : contentFiles) {
        if (path.starts_with("chapter") && path.ends_with(".xhtml")) {
            opf << "    <itemref idref=\"item" << itemCount << "\" />\n";
        }
        itemCount++;
    }
    
    opf << "  </spine>\n"
        << "  <guide>\n"
        << "    <reference type=\"cover\" title=\"表紙\" href=\"cover.xhtml\" />\n"
        << "    <reference type=\"toc\" title=\"目次\" href=\"toc.xhtml\" />\n"
        << "  </guide>\n"
        << "</package>\n";
    
    return opf.str();
}

std::string EpubOutputEngine::generateNcx(
    const core::document::Document& document,
    const EpubOutputOptions& options) {
    
    // NCXファイルの生成
    std::ostringstream ncx;
    
    ncx << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\" \"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\">\n"
        << "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">\n"
        << "  <head>\n"
        << "    <meta name=\"dtb:uid\" content=\"" << options.identifier << "\" />\n"
        << "    <meta name=\"dtb:depth\" content=\"1\" />\n"
        << "    <meta name=\"dtb:totalPageCount\" content=\"0\" />\n"
        << "    <meta name=\"dtb:maxPageNumber\" content=\"0\" />\n"
        << "  </head>\n"
        << "  <docTitle>\n"
        << "    <text>" << options.title << "</text>\n"
        << "  </docTitle>\n"
        << "  <docAuthor>\n"
        << "    <text>" << options.author << "</text>\n"
        << "  </docAuthor>\n"
        << "  <navMap>\n"
        << "    <navPoint id=\"navpoint-1\" playOrder=\"1\">\n"
        << "      <navLabel>\n"
        << "        <text>表紙</text>\n"
        << "      </navLabel>\n"
        << "      <content src=\"cover.xhtml\" />\n"
        << "    </navPoint>\n"
        << "    <navPoint id=\"navpoint-2\" playOrder=\"2\">\n"
        << "      <navLabel>\n"
        << "        <text>目次</text>\n"
        << "      </navLabel>\n"
        << "      <content src=\"toc.xhtml\" />\n"
        << "    </navPoint>\n";
    
    // チャプターの追加（実際には文書構造に基づいて生成）
    int navCount = 3;
    int chapterCount = 1;
    
    // 仮のチャプター数（実際には文書構造から取得）
    int totalChapters = 5;
    
    for (int i = 1; i <= totalChapters; i++) {
        std::ostringstream fileName;
        fileName << "chapter" << std::setw(3) << std::setfill('0') << i << ".xhtml";
        
        ncx << "    <navPoint id=\"navpoint-" << navCount << "\" playOrder=\"" << navCount << "\">\n"
            << "      <navLabel>\n"
            << "        <text>Chapter " << i << "</text>\n"
            << "      </navLabel>\n"
            << "      <content src=\"" << fileName.str() << "\" />\n"
            << "    </navPoint>\n";
        
        navCount++;
    }
    
    ncx << "  </navMap>\n"
        << "</ncx>\n";
    
    return ncx.str();
}

std::string EpubOutputEngine::generateContainer() {
    // container.xmlの生成
    std::ostringstream container;
    
    container << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
              << "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n"
              << "  <rootfiles>\n"
              << "    <rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\" />\n"
              << "  </rootfiles>\n"
              << "</container>\n";
    
    return container.str();
}

std::string EpubOutputEngine::createTempDir() {
    // 一時ディレクトリの作成
    std::string tempDir = std::filesystem::temp_directory_path().string() + "/japanese_typesetting_epub_" + std::to_string(std::rand());
    std::filesystem::create_directory(tempDir);
    return tempDir;
}

void EpubOutputEngine::removeTempDir(const std::string& dirPath) {
    // ディレクトリの削除
    std::filesystem::remove_all(dirPath);
}

std::string EpubOutputEngine::blocksToHtml(
    const std::vector<core::typesetting::TextBlock>& blocks,
    const EpubOutputOptions& options) {
    
    std::ostringstream html;
    core::unicode::UnicodeHandler unicodeHandler;
    
    // 各ブロックをHTML要素に変換
    for (const auto& block : blocks) {
        html << "  <div class=\"block\">\n";
        
        for (const auto& line : block.lines) {
            html << "    <p>";
            
            // UTF-32からUTF-8に変換
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            
            // 特殊文字のエスケープ
            for (char c : utf8Text) {
                switch (c) {
                    case '<': html << "&lt;"; break;
                    case '>': html << "&gt;"; break;
                    case '&': html << "&amp;"; break;
                    case '"': html << "&quot;"; break;
                    case '\'': html << "&#39;"; break;
                    default: html << c;
                }
            }
            
            html << "</p>\n";
        }
        
        html << "  </div>\n";
    }
    
    return html.str();
}

} // namespace output
} // namespace japanese_typesetting
