import terminal
import rdstdin
import linenoise

func initCLI() {.inline.} =
    discard historySetMaxLen(500)

proc launchCLI*() =
    initCLI()
    while true:
        var line: TaintedString
        let res = readLineFromStdin("> ", line)
        if res == false: continue
        if line.string == "exit": break
        if line.string == "clear":
            clearScreen()
