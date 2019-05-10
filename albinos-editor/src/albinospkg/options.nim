let doc = """
albinos_editor

Usage:
    albinos_editor (-h | --help)
    albinos_editor config create <name>
    albinos_editor --cli
    albinos_editor --gui

Options:
    --cli   Launch albinos_editor in cli mode.
    --gui   Launch albinos_editor in gui mode.
"""

import strutils, unicode
import docopt
import terminal
import typeinfo

proc parseOpt*: Table[system.string, docopt.Value] =
    return docopt(doc)
