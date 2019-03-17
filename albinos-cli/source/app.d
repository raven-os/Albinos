import std.stdio;
import std.string;
import std.socket : UnixAddress, Socket, SocketType, AddressFamily, SocketOSException;
import std.uni : isWhite;
import std.process : executeShell;

///
class Client
{
	///
	this(string socket_path)
	{
		this.address_ = new UnixAddress(socket_path);
		this.socket_ = new Socket(AddressFamily.UNIX, SocketType.STREAM);
		socket_.connect(this.address_);
	}

	private UnixAddress address_;
	private Socket socket_;
}

///
class CLI
{
	this(string socket_path)
	{
		try
		{
			client_ = new Client(socket_path);
		}
		catch (SocketOSException e)
		{
			writeln(stderr, e.msg);
			is_running_ = false;
		}
	}
	///
	public void run_program()
	{
		if (!is_running_)
			return;
		write("> ");
		string line;
		while (((line = stdin.readln().strip("\n \t")) !is null))
		{
			if (line == "exit")
			{
				break;
			}
			auto splited_line = line.split!isWhite;
			//! Command with argument
			if (splited_line.length >= 2)
			{
				auto args = splited_line[1 .. splited_line.length];
				auto cmd = splited_line[0];
				writeln("cmd -> [", cmd, "] args -> ", args);
				switch (cmd)
				{
				default:
					immutable auto result = line.dup.executeShell;
					result.output.write;
					break;
				}
			}
			//! Command without argument
			else
			{
				switch (line)
				{
				default:
					immutable auto result = line.dup.executeShell;
					result.output.write;
					break;
				}
			}
			write("> ");
		}
	}

	private Client client_;
	private bool is_running_ = true;
}

void main()
{
	auto cli = new CLI("/tmp/raven-os_service_albinos.sock");
	cli.run_program;
}
