/**
 * @file typesetting.h
 * @brief 組版処理のヘッダファイル
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_TYPESETTING_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_TYPESETTING_H

#include <string>
#include <vector>
#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @enum ProhibitionRule
 * @brief 禁則処理のルール種類
 */
enum class ProhibitionRule {
    LineStart,      ///< 行頭禁則
    LineEnd,        ///< 行末禁則
    Hanging,        ///< ぶら下がり処理
    WordBreak       ///< 単語分割禁止
};

/**
 * @class TypesettingEngine
 * @brief 日本語組版エンジンの基本クラス
 * 
 * JIS X 4051に準拠した日本語組版処理を行うエンジン
 */
class TypesettingEngine {
public:
    /**
     * @brief コンストラクタ
     */
    TypesettingEngine();

    /**
     * @brief デストラクタ
     */
    virtual ~TypesettingEngine();

    /**
     * @brief 文書を処理して組版結果を生成
     * @param document 処理対象の文書
     * @return 処理が成功したかどうか
     */
    virtual bool process(const document::Document& document);

    /**
     * @brief 禁則処理を有効/無効に設定
     * @param rule 禁則ルールの種類
     * @param enabled 有効にする場合はtrue
     */
    void setProhibitionRule(ProhibitionRule rule, bool enabled);

    /**
     * @brief 禁則処理が有効かどうかを取得
     * @param rule 禁則ルールの種類
     * @return 有効な場合はtrue
     */
    bool isProhibitionRuleEnabled(ProhibitionRule rule) const;

    /**
     * @brief 行頭禁則文字を追加
     * @param character 行頭禁則文字
     */
    void addLineStartProhibitedCharacter(char32_t character);

    /**
     * @brief 行末禁則文字を追加
     * @param character 行末禁則文字
     */
    void addLineEndProhibitedCharacter(char32_t character);

    /**
     * @brief 文字詰め処理を設定
     * @param enabled 有効にする場合はtrue
     */
    void setCharacterFitting(bool enabled);

    /**
     * @brief 文字詰め処理が有効かどうかを取得
     * @return 有効な場合はtrue
     */
    bool isCharacterFittingEnabled() const;

protected:
    /**
     * @brief 行頭禁則処理を適用
     * @param text 処理対象のテキスト
     * @return 処理後のテキスト
     */
    virtual std::string applyLineStartProhibition(const std::string& text);

    /**
     * @brief 行末禁則処理を適用
     * @param text 処理対象のテキスト
     * @return 処理後のテキスト
     */
    virtual std::string applyLineEndProhibition(const std::string& text);

    /**
     * @brief ぶら下がり処理を適用
     * @param text 処理対象のテキスト
     * @return 処理後のテキスト
     */
    virtual std::string applyHangingProhibition(const std::string& text);

private:
    std::vector<char32_t> m_lineStartProhibitedChars; ///< 行頭禁則文字リスト
    std::vector<char32_t> m_lineEndProhibitedChars;   ///< 行末禁則文字リスト
    bool m_prohibitionRules[4];                       ///< 禁則ルールの有効/無効状態
    bool m_characterFittingEnabled;                   ///< 文字詰め処理の有効/無効状態
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_TYPESETTING_H
