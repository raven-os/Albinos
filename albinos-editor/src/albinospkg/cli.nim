import terminal
import rdstdin
import linenoise

proc launchCLI*() =
  while true:
    var line: TaintedString
    let res = readLineFromStdin("> ", line)
    if res == false: continue
    if line.string == "exit": break
    if line.string == "clear":
      clearScreen()
