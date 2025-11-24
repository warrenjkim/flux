#pragma once

namespace flux {

// ascii table. see https://ascii-code.com
//
// NOTE: does not map 128-255
enum class Key {
  kNone = 0,

  // control characters (0-31)
  kCtrlA = 1,
  kCtrlB,
  kCtrlC,
  kCtrlD,
  kCtrlE,
  kCtrlF,
  kCtrlG,
  kCtrlH,
  kCtrlI,
  kCtrlJ,
  kCtrlK,
  kCtrlL,
  kCtrlM,
  kCtrlN,
  kCtrlO,
  kCtrlP,
  kCtrlQ,
  kCtrlR,
  kCtrlS,
  kCtrlT,
  kCtrlU,
  kCtrlV,
  kCtrlW,
  kCtrlX,
  kCtrlY,
  kCtrlZ,
  kEscape,  // 27

  // printable ascii (32-126)
  kSpace = 32,
  kBang,        // !
  kQuote,       // "
  kPound,       // #
  kDollar,      // $
  kPercent,     // %
  kAmpersand,   // &
  kApostrophe,  // '
  kLeftParen,   // (
  kRightParen,  // )
  kAsterisk,    // *
  kPlus,        // +
  kComma,       // ,
  kMinus,       // -
  kPeriod,      // .
  kSlash,       // /

  k0,
  k1,
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,

  kColon,      // :
  kSemicolon,  // ;
  kLess,       //
  kEqual,      // =
  kGreater,    // >
  kQuestion,   // ?
  kAt,         // @

  kA,
  kB,
  kC,
  kD,
  kE,
  kF,
  kG,
  kH,
  kI,
  kJ,
  kK,
  kL,
  kM,
  kN,
  kO,
  kP,
  kQ,
  kR,
  kS,
  kT,
  kU,
  kV,
  kW,
  kX,
  kY,
  kZ,

  kLeftBracket,
  kBackslash,
  kRightBracket,
  kCaret,
  kUnderscore,
  kGrave,

  ka,
  kb,
  kc,
  kd,
  ke,
  kf,
  kg,
  kh,
  ki,
  kj,
  kk,
  kl,
  km,
  kn,
  ko,
  kp,
  kq,
  kr,
  ks,
  kt,
  ku,
  kv,
  kw,
  kx,
  ky,
  kz,

  kLeftBrace,
  kPipe,
  kRightBrace,
  kTilde,

  kBackspace,

  kTab = kCtrlI,
  kEnter = kCtrlM,

  // escape sequences
  kArrowUp = 1000,
  kArrowDown,
  kArrowLeft,
  kArrowRight,

  // TODO(function keys)  = 2000

  // TODO(modified arrows) = 3000
};

}  // namespace flux
