
open LccTypes


(*recursively advances the position in the input string until finds 
a non space character*)
(*let rec skip_whitespace pos input =
    if pos < String.length input && (input.[pos] = ' '|| input.[pos] = '\t'||input.[pos] = '\n') then
        skip_whitespace(pos + 1) input
    else
        pos*)

(*pattern matching, *)
let lex_lambda input =

    (*end of the string have to have a loop*)
    
     
    (*end recurssive call, position of input if the position is greater
    than the input length then append EOF*)

    let rec aux input  = 
      
      if String.length input == 0 then
        [Lambda_EOF]
      (*return the eof*)
        
      else
        
        if Re.execp(Re.compile(Re.Posix.re"^[ \t\n\r]")) input then
          aux(String.sub input 1 (String.length input - 1))
        
        
        else if Re.execp (Re.compile(Re.Posix.re "^L")) input then 
          Lambda_Lambda::aux (String.sub input 1(String.length input - 1)) 
        
        else if Re.execp (Re.compile(Re.Posix.re "^\\(")) input then
          Lambda_LParen::aux(String.sub input 1 (String.length input - 1))
        
        else if Re.execp(Re.compile(Re.Posix.re "^\\)")) input then
          Lambda_RParen::aux(String.sub input 1 (String.length input -1))
        
        else if Re.execp(Re.compile(Re.Posix.re "^\\.")) input then
          Lambda_Dot::aux(String.sub input 1 (String.length input -1 ))
        
        else if Re.execp(Re.compile(Re.Posix.re "^[a-z]")) input then
          let var = String.sub input 0 1 in
          Lambda_Var (var):: aux(String.sub input 1 (String.length input - 1))

        else 
          raise (Failure "tokenizing failed")
      in 
      aux input


let lex_engl input = 
  let rec aux input = 
      (*let pos = skip_whitespace pos input in *)
      (*if pos >= String.length input then
        [Engl_EOF]*)
      if String.length input = 0 then
        [Engl_EOF]
      else

        
        if Re.execp(Re.compile(Re.Posix.re "^ ")) input then
           aux(String.sub input 1 (String.length input - 1))

        else if Re.execp(Re.compile(Re.Posix.re "^\(")) input then
          Engl_LParen::aux(String.sub input 1 (String.length input-1))
        
        else if Re.execp(Re.compile(Re.Posix.re "^\)")) input then
          Engl_RParen::aux(String.sub input 1 (String.length input - 1))
        
        else if Re.execp(Re.compile(Re.Posix.re "^true")) input then
          Engl_True::aux(String.sub input 4 (String.length input - 4))
        
        else if Re.execp(Re.compile(Re.Posix.re "^false")) input then
          Engl_False::aux(String.sub input 5 (String.length input - 5))
        
        else if Re.execp(Re.compile(Re.Posix.re "^if")) input then
          Engl_If::aux(String.sub input 2 (String.length input - 2))

        else if Re.execp(Re.compile(Re.Posix.re "^else")) input then
          Engl_Else::aux(String.sub input 4(String.length input - 4))
        
        else if Re.execp(Re.compile(Re.Posix.re "^then")) input then
          Engl_Then::aux(String.sub input 4(String.length input - 4))
        
        else if Re.execp(Re.compile(Re.Posix.re "^and")) input then
          Engl_And::aux(String.sub input 3(String.length input - 3))
        
        else if Re.execp(Re.compile(Re.Posix.re "^or")) input then
          Engl_Or::aux(String.sub input 2(String.length input - 2))
        
        else if Re.execp(Re.compile(Re.Posix.re "^not")) input then
          Engl_Not::aux(String.sub input 3(String.length input - 3))

       
        else 
          raise (Failure "tokenizing failed")
    in 
    aux input
