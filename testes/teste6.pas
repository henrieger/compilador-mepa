program cmdIf (input, output);
var i, j: integer;       
begin                   
   read(j);                 
   i:=0;                   
   while (i < j) do        
   begin                   
      if (i div 2 * 2 = i)  
        then
        begin
          write(i,0);
          i:=i
        end 
        else
        begin
          write(i,1);
          j:=j
        end;
      i := i+1                    
   end                    
end. 

