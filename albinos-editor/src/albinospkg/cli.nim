import linenoise

proc launchCLI*() =
    discard historySetMaxLen(500)
    while true:
        let res = readLine("> ")
        if res == "exit": break
        if res == "clear":
            clearScreen()
        discard historyAdd(res)
        free(res)
