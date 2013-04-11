program HelloWorld;
var a,b,c,x,y,z,min:Integer;

procedure findMin(x, y, z: integer; var m: integer); 
begin
  if x < y then
      m:= x
   else
      m:= y;
   if z < m then
      m:= z;
 end;
 
begin

if a>c then 
	c:=100 
else 
	c:=-200;
	
b:=90;

for a:= 0 to 60 do 
	b:=b-1;

c:=1;

while c<b do
begin 
c:=c*3+1;
end;

repeat
 read (a);
write (7);
a:=a+1;
  
until  a>c

end.