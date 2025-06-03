/**
 * @file typesetting_rules.h
 * @brief 日本語組版ルールの定義
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_RULES_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_RULES_H

#include <string>
#include <vector>
#include <set>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @class TypesettingRules
 * @brief JIS X 4051に準拠した日本語組版ルールを定義するクラス
 */
class TypesettingRules {
public:
    /**
     * @brief コンストラクタ
     */
    TypesettingRules();

    /**
     * @brief デストラクタ
     */
    ~TypesettingRules();

    /**
     * @brief 行頭禁則文字を追加
     * @param character 行頭禁則文字
     */
    void addLineStartProhibitedCharacter(char32_t character);

    /**
     * @brief 行頭禁則文字かどうかを判定
     * @param character 判定する文字
     * @return 行頭禁則文字の場合はtrue
     */
    bool isLineStartProhibited(char32_t character) const;

    /**
     * @brief 行頭禁則文字のセットを取得
     * @return 行頭禁則文字のセット
     */
    const std::set<char32_t>& getLineStartProhibitedCharacters() const;

    /**
     * @brief 行末禁則文字を追加
     * @param character 行末禁則文字
     */
    void addLineEndProhibitedCharacter(char32_t character);

    /**
     * @brief 行末禁則文字かどうかを判定
     * @param character 判定する文字
     * @return 行末禁則文字の場合はtrue
     */
    bool isLineEndProhibited(char32_t character) const;

    /**
     * @brief 行末禁則文字のセットを取得
     * @return 行末禁則文字のセット
     */
    const std::set<char32_t>& getLineEndProhibitedCharacters() const;

    /**
     * @brief 分離禁止文字を追加
     * @param character 分離禁止文字
     */
    void addInseparableCharacter(char32_t character);

    /**
     * @brief 分離禁止文字かどうかを判定
     * @param character 判定する文字
     * @return 分離禁止文字の場合はtrue
     */
    bool isInseparable(char32_t character) const;

    /**
     * @brief 分離禁止文字のセットを取得
     * @return 分離禁止文字のセット
     */
    const std::set<char32_t>& getInseparableCharacters() const;

    /**
     * @brief ぶら下げ対象文字を追加
     * @param character ぶら下げ対象文字
     */
    void addHangingCharacter(char32_t character);

    /**
     * @brief ぶら下げ対象文字かどうかを判定
     * @param character 判定する文字
     * @return ぶら下げ対象文字の場合はtrue
     */
    bool isHangingCharacter(char32_t character) const;

    /**
     * @brief ぶら下げ対象文字のセットを取得
     * @return ぶら下げ対象文字のセット
     */
    const std::set<char32_t>& getHangingCharacters() const;

    /**
     * @brief JIS X 4051に準拠したデフォルトの禁則ルールを設定
     */
    void setDefaultJisX4051Rules();

    /**
     * @brief 禁則ルールをファイルから読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadFromFile(const std::string& filePath);

    /**
     * @brief 禁則ルールをファイルに保存
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool saveToFile(const std::string& filePath) const;

private:
    std::set<char32_t> m_lineStartProhibitedChars;  ///< 行頭禁則文字のセット
    std::set<char32_t> m_lineEndProhibitedChars;    ///< 行末禁則文字のセット
    std::set<char32_t> m_inseparableChars;          ///< 分離禁止文字のセット
    std::set<char32_t> m_hangingChars;              ///< ぶら下げ対象文字のセット
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_RULES_H
