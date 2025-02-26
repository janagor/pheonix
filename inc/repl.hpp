#pragma once

#include "evaluator.hpp"
#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <vector>

namespace pheonix::repl {

const std::string_view PHEONIX =
    R"(                          j    |
  .                         u _ (
' c                        .   t t
f Y            u  v            t r
 n  u           ccczc      t  t:.  t
  uu uv        vzz zzc    tr jj  ft
 v  vvv`cxcc   zzn   X  `nnnx xrr'
X  vccczczzzzzXXXj    vcvvvuuuunnn. X
 `YX    XzzXXXYYYYzU  Xzzzzzc-  'U
         XfYYYYUUUUUU.UUYYYYX  {
          Y    UUJJJJCCC'  I `
               JJCCCCC0Q
              J'ClLCLL  x0
                 LLLQ
            . LL QQQ
          x L^ rQQ1.
         u    Q0.0
        cc   0  O
       `X   Ov  ff c
           . zz  Z   zz.
            'UU Y mmm.YXX
             CC C'mmm   U
             0.0Q.  tm
               OO
______ _        }             _
| ___ \ |                    (_)
| |_/ / |__   ___  ___  _ __  ___  __
|  __/| '_ \ / _ \/ _ \| '_ \| \ \/ /
| |   | | | |  __/ (_) | | | | |>  < 
\_|   |_| |_|\___|\___/|_| |_|_/_/\_\
)";

void repl();

} // namespace pheonix::repl
