program PROGRAM_NAME;
	var num : integer;
		name : string;
	function fibonacci(id: string; i : integer) : integer;
		function fibonacciHelper(n : integer) : integer;
			{ fibonacciHelper(25) == 75025 }
			begin
				if(n <= 1) then
					fibonacciHelper := n
				else
					fibonacciHelper := fibonacciHelper(n-1) + fibonacciHelper(n-2)
			end;
		begin
			if (i < 0.0) or (i > 25) then
				begin
					Write('Yo, i is too small or too large, ', name, '.  You passed ', i, '!');
					fibonacci := -8008
				end
			else
				fibonacci := fibonacciHelper(i)
		end;
	begin
		Write('Enter name: ');
		Read(name);
		Write('Enter a number, n, to find the n-th fibonacci number: ');
		Read(num);
		Write(num, '! = ');
		Writeln(fibonacci(name, num))
	end.
