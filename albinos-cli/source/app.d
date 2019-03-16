import std.stdio;
import std.string;
import std.socket : UnixAddress, Socket, SocketType, AddressFamily;
import std.uni : isWhite;

///
class Client
{
	///
	this(string socket_path)
	{
		this.address_= new UnixAddress(socket_path);
		this.socket_ = new Socket(AddressFamily.UNIX, SocketType.STREAM);
		socket_.connect(this.address_);
	}
	
	private UnixAddress address_;
	private Socket socket_;
}

///
class CLI 
{
	///
	public void run_program()
	{	
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
}

void main()
{
	auto cli = new CLI();
	cli.run_program;
}