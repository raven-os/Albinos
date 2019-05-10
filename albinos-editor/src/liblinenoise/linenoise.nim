##  linenoise.h -- VERSION 1.0
##
##  Guerrilla line editing library against the idea that a line editing lib
##  needs to be 20,000 lines of C code.
##
##  See linenoise.c for more information.
##
##  ------------------------------------------------------------------------
##
##  Copyright (c) 2010-2014, Salvatore Sanfilippo <antirez at gmail dot com>
##  Copyright (c) 2010-2013, Pieter Noordhuis <pcnoordhuis at gmail dot com>
##
##  All rights reserved.
##
##  Redistribution and use in source and binary forms, with or without
##  modification, are permitted provided that the following conditions are
##  met:
##
##   *  Redistributions of source code must retain the above copyright
##      notice, this list of conditions and the following disclaimer.
##
##   *  Redistributions in binary form must reproduce the above copyright
##      notice, this list of conditions and the following disclaimer in the
##      documentation and/or other materials provided with the distribution.
##
##  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
##  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
##  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
##  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
##  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
##  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
##  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
##  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
##  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
##  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
##  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

{.deadCodeElim: on.}
when defined(linux):
  const
    linenoisedll* = "liblinenoise.so"
type
  linenoiseCompletions* {.bycopy.} = object
    len*: csize
    cvec*: cstringArray

  linenoiseCompletionCallback* = proc (a1: cstring; a2: ptr linenoiseCompletions) {.
      cdecl.}
  linenoiseHintsCallback* = proc (a1: cstring; color: ptr cint; bold: ptr cint): cstring {.
      cdecl.}
  linenoiseFreeHintsCallback* = proc (a1: pointer) {.cdecl.}

proc linenoiseSetCompletionCallback*(a1: ptr linenoiseCompletionCallback) {.cdecl,
    importc: "linenoiseSetCompletionCallback", dynlib: linenoisedll.}
proc linenoiseSetHintsCallback*(a1: ptr linenoiseHintsCallback) {.cdecl,
    importc: "linenoiseSetHintsCallback", dynlib: linenoisedll.}
proc linenoiseSetFreeHintsCallback*(a1: ptr linenoiseFreeHintsCallback) {.cdecl,
    importc: "linenoiseSetFreeHintsCallback", dynlib: linenoisedll.}
proc linenoiseAddCompletion*(a1: ptr linenoiseCompletions; a2: cstring) {.cdecl,
    importc: "linenoiseAddCompletion", dynlib: linenoisedll.}
proc linenoise*(prompt: cstring): cstring {.cdecl, importc: "linenoise",
                                        dynlib: linenoisedll.}
proc linenoiseFree*(`ptr`: pointer) {.cdecl, importc: "linenoiseFree",
                                   dynlib: linenoisedll.}
proc linenoiseHistoryAdd*(line: cstring): cint {.cdecl,
    importc: "linenoiseHistoryAdd", dynlib: linenoisedll.}
proc linenoiseHistorySetMaxLen*(len: cint): cint {.cdecl,
    importc: "linenoiseHistorySetMaxLen", dynlib: linenoisedll.}
proc linenoiseHistorySave*(filename: cstring): cint {.cdecl,
    importc: "linenoiseHistorySave", dynlib: linenoisedll.}
proc linenoiseHistoryLoad*(filename: cstring): cint {.cdecl,
    importc: "linenoiseHistoryLoad", dynlib: linenoisedll.}
proc linenoiseClearScreen*() {.cdecl, importc: "linenoiseClearScreen",
                             dynlib: linenoisedll.}
proc linenoiseSetMultiLine*(ml: cint) {.cdecl, importc: "linenoiseSetMultiLine",
                                     dynlib: linenoisedll.}
proc linenoisePrintKeyCodes*() {.cdecl, importc: "linenoisePrintKeyCodes",
                               dynlib: linenoisedll.}