/**
 * @file style.h
 * @brief スタイル定義の詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_STYLE_STYLE_H
#define JAPANESE_TYPESETTING_CORE_STYLE_STYLE_H

#include <string>
#include <map>
#include <vector>

namespace japanese_typesetting {
namespace core {
namespace style {

/**
 * @enum TextAlignment
 * @brief テキスト配置の種類
 */
enum class TextAlignment {
    Left,       ///< 左揃え（横書き）または上揃え（縦書き）
    Right,      ///< 右揃え（横書き）または下揃え（縦書き）
    Center,     ///< 中央揃え
    Justify     ///< 両端揃え
};

/**
 * @enum LineBreakMode
 * @brief 改行モードの種類
 */
enum class LineBreakMode {
    Normal,     ///< 通常の改行
    Strict,     ///< 厳格な禁則処理
    Loose       ///< 緩やかな禁則処理
};

/**
 * @class Style
 * @brief 文書のスタイルを定義するクラス
 */
class Style {
public:
    /**
     * @brief コンストラクタ
     */
    Style();

    /**
     * @brief デストラクタ
     */
    ~Style();

    /**
     * @brief フォントファミリーを設定
     * @param fontFamily フォントファミリー名
     */
    void setFontFamily(const std::string& fontFamily);

    /**
     * @brief フォントファミリーを取得
     * @return フォントファミリー名
     */
    std::string getFontFamily() const;

    /**
     * @brief フォントサイズを設定
     * @param fontSize フォントサイズ（ポイント）
     */
    void setFontSize(double fontSize);

    /**
     * @brief フォントサイズを取得
     * @return フォントサイズ（ポイント）
     */
    double getFontSize() const;

    /**
     * @brief 行の高さを設定
     * @param lineHeight 行の高さ（倍率）
     */
    void setLineHeight(double lineHeight);

    /**
     * @brief 行の高さを取得
     * @return 行の高さ（倍率）
     */
    double getLineHeight() const;

    /**
     * @brief テキスト配置を設定
     * @param alignment テキスト配置の種類
     */
    void setTextAlignment(TextAlignment alignment);

    /**
     * @brief テキスト配置を取得
     * @return テキスト配置の種類
     */
    TextAlignment getTextAlignment() const;

    /**
     * @brief 改行モードを設定
     * @param mode 改行モードの種類
     */
    void setLineBreakMode(LineBreakMode mode);

    /**
     * @brief 改行モードを取得
     * @return 改行モードの種類
     */
    LineBreakMode getLineBreakMode() const;

    /**
     * @brief 文字間隔を設定
     * @param spacing 文字間隔（em単位）
     */
    void setCharacterSpacing(double spacing);

    /**
     * @brief 文字間隔を取得
     * @return 文字間隔（em単位）
     */
    double getCharacterSpacing() const;

    /**
     * @brief 単語間隔を設定
     * @param spacing 単語間隔（em単位）
     */
    void setWordSpacing(double spacing);

    /**
     * @brief 単語間隔を取得
     * @return 単語間隔（em単位）
     */
    double getWordSpacing() const;

    /**
     * @brief 段落前の余白を設定
     * @param margin 段落前の余白（em単位）
     */
    void setParagraphSpacingBefore(double margin);

    /**
     * @brief 段落前の余白を取得
     * @return 段落前の余白（em単位）
     */
    double getParagraphSpacingBefore() const;

    /**
     * @brief 段落後の余白を設定
     * @param margin 段落後の余白（em単位）
     */
    void setParagraphSpacingAfter(double margin);

    /**
     * @brief 段落後の余白を取得
     * @return 段落後の余白（em単位）
     */
    double getParagraphSpacingAfter() const;

    /**
     * @brief 段落の最初の行のインデントを設定
     * @param indent インデント量（em単位）
     */
    void setFirstLineIndent(double indent);

    /**
     * @brief 段落の最初の行のインデントを取得
     * @return インデント量（em単位）
     */
    double getFirstLineIndent() const;

    /**
     * @brief 太字設定を有効/無効にする
     * @param bold 太字にする場合はtrue
     */
    void setBold(bool bold);

    /**
     * @brief 太字設定を取得
     * @return 太字の場合はtrue
     */
    bool isBold() const;

    /**
     * @brief 斜体設定を有効/無効にする
     * @param italic 斜体にする場合はtrue
     */
    void setItalic(bool italic);

    /**
     * @brief 斜体設定を取得
     * @return 斜体の場合はtrue
     */
    bool isItalic() const;

    /**
     * @brief 下線設定を有効/無効にする
     * @param underline 下線を付ける場合はtrue
     */
    void setUnderline(bool underline);

    /**
     * @brief 下線設定を取得
     * @return 下線がある場合はtrue
     */
    bool isUnderline() const;

    /**
     * @brief カスタムプロパティを設定
     * @param key プロパティのキー
     * @param value プロパティの値
     */
    void setProperty(const std::string& key, const std::string& value);

    /**
     * @brief カスタムプロパティを取得
     * @param key プロパティのキー
     * @return プロパティの値、存在しない場合は空文字列
     */
    std::string getProperty(const std::string& key) const;

    /**
     * @brief スタイルをファイルから読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadFromFile(const std::string& filePath);

    /**
     * @brief スタイルをファイルに保存
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool saveToFile(const std::string& filePath) const;

private:
    std::string m_fontFamily;             ///< フォントファミリー
    double m_fontSize;                    ///< フォントサイズ（ポイント）
    double m_lineHeight;                  ///< 行の高さ（倍率）
    TextAlignment m_textAlignment;        ///< テキスト配置
    LineBreakMode m_lineBreakMode;        ///< 改行モード
    double m_characterSpacing;            ///< 文字間隔（em単位）
    double m_wordSpacing;                 ///< 単語間隔（em単位）
    double m_paragraphSpacingBefore;      ///< 段落前の余白（em単位）
    double m_paragraphSpacingAfter;       ///< 段落後の余白（em単位）
    double m_firstLineIndent;             ///< 段落の最初の行のインデント（em単位）
    bool m_bold;                          ///< 太字フラグ
    bool m_italic;                        ///< 斜体フラグ
    bool m_underline;                     ///< 下線フラグ
    std::map<std::string, std::string> m_properties; ///< カスタムプロパティ
};

} // namespace style
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_STYLE_STYLE_H
