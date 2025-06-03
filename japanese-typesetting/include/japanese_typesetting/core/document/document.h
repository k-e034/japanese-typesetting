/**
 * @file document.h
 * @brief 文書構造の詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_DOCUMENT_DOCUMENT_H
#define JAPANESE_TYPESETTING_CORE_DOCUMENT_DOCUMENT_H

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace japanese_typesetting {
namespace core {
namespace document {

/**
 * @class Section
 * @brief 文書のセクションを表すクラス
 */
class Section {
public:
    /**
     * @brief コンストラクタ
     * @param title セクションのタイトル
     */
    Section(const std::string& title = "");

    /**
     * @brief デストラクタ
     */
    ~Section();

    /**
     * @brief セクションのタイトルを取得
     * @return セクションのタイトル
     */
    std::string getTitle() const;

    /**
     * @brief セクションのタイトルを設定
     * @param title 設定するタイトル
     */
    void setTitle(const std::string& title);

    /**
     * @brief セクションの内容を取得
     * @return セクションの内容
     */
    std::string getContent() const;

    /**
     * @brief セクションの内容を設定
     * @param content 設定する内容
     */
    void setContent(const std::string& content);

    /**
     * @brief 子セクションを追加
     * @param section 追加するセクション
     */
    void addChildSection(Section* section);

    /**
     * @brief 子セクションを取得
     * @param index 子セクションのインデックス
     * @return 子セクションへのポインタ、存在しない場合はnullptr
     */
    Section* getChildSection(size_t index) const;

    /**
     * @brief 子セクションの数を取得
     * @return 子セクションの数
     */
    size_t getChildSectionCount() const;

    /**
     * @brief メタデータを設定
     * @param key メタデータのキー
     * @param value メタデータの値
     */
    void setMetadata(const std::string& key, const std::string& value);

    /**
     * @brief メタデータを取得
     * @param key メタデータのキー
     * @return メタデータの値、存在しない場合は空文字列
     */
    std::string getMetadata(const std::string& key) const;

private:
    std::string m_title;                      ///< セクションのタイトル
    std::string m_content;                    ///< セクションの内容
    std::vector<Section*> m_childSections;    ///< 子セクションのリスト
    std::map<std::string, std::string> m_metadata; ///< メタデータ
};

/**
 * @class Document
 * @brief 文書全体を表すクラス
 */
class Document {
public:
    /**
     * @brief デフォルトコンストラクタ
     */
    Document();

    /**
     * @brief パラメータ付きコンストラクタ
     * @param title 文書のタイトル
     * @param author 文書の著者
     * @param vertical 縦書きの場合はtrue、横書きの場合はfalse
     */
    Document(const std::string& title, const std::string& author, bool vertical = true);

    /**
     * @brief デストラクタ
     */
    ~Document();

    /**
     * @brief 文書のタイトルを取得
     * @return 文書のタイトル
     */
    std::string getTitle() const;

    /**
     * @brief 文書のタイトルを設定
     * @param title 設定するタイトル
     */
    void setTitle(const std::string& title);

    /**
     * @brief 文書の著者を取得
     * @return 文書の著者
     */
    std::string getAuthor() const;

    /**
     * @brief 文書の著者を設定
     * @param author 設定する著者
     */
    void setAuthor(const std::string& author);

    /**
     * @brief 文書が縦書きかどうかを取得
     * @return 縦書きの場合はtrue、横書きの場合はfalse
     */
    bool isVertical() const;

    /**
     * @brief 文書の縦書き/横書きを設定
     * @param vertical 縦書きの場合はtrue、横書きの場合はfalse
     */
    void setVertical(bool vertical);

    /**
     * @brief セクションを追加
     * @param section 追加するセクション
     */
    void addSection(Section* section);

    /**
     * @brief セクションを取得
     * @param index セクションのインデックス
     * @return セクションへのポインタ、存在しない場合はnullptr
     */
    Section* getSection(size_t index) const;

    /**
     * @brief セクションの数を取得
     * @return セクションの数
     */
    size_t getSectionCount() const;

    /**
     * @brief メタデータを設定
     * @param key メタデータのキー
     * @param value メタデータの値
     */
    void setMetadata(const std::string& key, const std::string& value);

    /**
     * @brief メタデータを取得
     * @param key メタデータのキー
     * @return メタデータの値、存在しない場合は空文字列
     */
    std::string getMetadata(const std::string& key) const;

    /**
     * @brief 文書をファイルから読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadFromFile(const std::string& filePath);

    /**
     * @brief 文書をファイルに保存
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool saveToFile(const std::string& filePath) const;

private:
    std::string m_title;                  ///< 文書のタイトル
    std::string m_author;                 ///< 文書の著者
    bool m_vertical;                      ///< 縦書きフラグ
    std::vector<Section*> m_sections;     ///< セクションのリスト
    std::map<std::string, std::string> m_metadata; ///< メタデータ
};

} // namespace document
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_DOCUMENT_DOCUMENT_H
