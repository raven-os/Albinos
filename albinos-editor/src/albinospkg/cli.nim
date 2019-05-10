import ../liblinenoise/linenoise

proc launchCLI*() =
    discard linenoiseHistorySetMaxLen(500)
    while true:
        let res = linenoise("> ")
        if res == "exit": break
        if res == "clear":
            linenoiseClearScreen()
        discard linenoiseHistoryAdd(res)
        linenoiseFree(res)
