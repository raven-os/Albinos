import std.stdio;
import std.string;

void main()
{
	import std.uni : isWhite;
	write("> ");
	string line;
	while ((line = stdin.readln().stripRight("\n")) !is null) {	
		if (line == "exit")
			break;
		auto splited_line = line.split!isWhite;
		if (splited_line.length >= 2) {
			auto args = splited_line[1..splited_line.length];
			auto cmd = splited_line[0];
			writeln("cmd -> [", cmd, "]");
			writeln("args -> ", args);
		}
		write("> ");
	}
}