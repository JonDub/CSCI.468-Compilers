{http://hypertextbookshop.com/transPL/Contents/01_Topics/05_Chapter_5/01_Section_1/02_Page_2_-_Run_Time_Memory_and_Symbol_Table.html}
{ Slightly modified to work with microPascal. }
program Main;
	var A, B, C: Integer;
	Ch: string;
	D : float;
	procedure Fred;
		var A, X: Integer;
		procedure Mary;
			var B, X, C: Integer;
			begin { Mary }
				B := 1; X := 2; C := 3; 
				A := B + X * C;
			end; { Mary }
		begin { Fred; }
			A := 75; C := 100; 
{ EDIT: X must be assigned a value. } X := 1;
			C := A * X - B;
			Mary;
		end; { Fred }
	procedure Jack;
		var B, Q: float;
			A: Integer;
		begin { Jack }
			B := 3.14; Q := 14.0;
			D := B / Q;
			A := 3;
		end; { Jack }
	begin { Main }
		A := 10; B := 20; C := 30; D := 15.5; Ch := 'x';
		Fred;
		Jack;
	end. { Main }
