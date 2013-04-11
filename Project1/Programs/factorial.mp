program PROGRAM_NAME;
	var num : integer;
		name : string;
	function factorial(id: string; i : integer) : integer;
		function factorialHelper(n : integer) : integer;
			begin
				if(n <= 1) then
					factorialHelper := 1
				else
					factorialHelper := n * factorialHelper(n - 1)
			end;
		begin
			if i < 0 then
				begin
					Write('Yo, i must be at least zero, ', name, '.  You passed ', i, '!');
					factorial := -8008
				end
			else
				factorial := factorialHelper(i)
		end;
	begin
		Write('Enter name: ');
		Read(name);
		Write('Enter a number to find the factorial of: ');
		Read(num);
		Write(num, '! = ');
		Writeln(factorial(name, num))
	end.
