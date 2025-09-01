open LccTypes 

let match_token (toks : 'a list) (tok : 'a) : 'a list =
  match toks with
  | [] -> raise (Failure("parsing failed"))
  | h::t when h = tok -> t
  | h::_ -> raise (Failure("parsing failed"))

let lookahead toks = match toks with
   h::t -> h
  | _ -> raise (Failure("parsing failed"))


(* Write your code below *)
(*(Lx.e) -> after dot need to parse the tree*)
(*t is the remaining*)
(*let parse_lambda toks = 
  let rec parse_lambda toks = 
    match toks with
    |[]->raise (Failure "parsing failed")
    

  match toks with
  | Lambda_Var (v)::t -> (Var(v), t)
  | Lambda_LParen::Lambda_Lambda::Lambda_Var(x)::Lambda_Dot::t::let(expr, r) = parse_lambda t in 
    match r with 
    |Lambda_RParen::t->(Fun(v,e))
  | Lambda_LParen::let(expr, r) = parse_lambda t in 
    let(expr2, r2) = parse_lambda r in 
    match with 
    |Lambda_RParen::t ->(Fun(v,e))*)

(*let parse_lambda toks = 
  let rec parse_lambda toks = 
    match toks with
    |Lambda_Var v::rest -> (Var v, rest)
    |Lambda_LParen::Lambda_Lambda::Lambda_Var v::Lambda_Dot::rest->
      let (e, rest') = parse_lambda rest in
      (match rest' with
      |Lambda_RParen::rest''->(Func(v, e), rest'')
      |_->raise (Failure "parsing failed"))

    |Lambda_LParen::rest->
      let (e1, rest')= parse_lambda rest in 
      let (e2, rest'') = parse_lambda rest' in
      (match rest'' with
      |Lambda_RParen::rest'''->(Application(e1,e2), rest''')
      |_->raise(Failure "parsing failed"))
    |_->raise(Failure "parsing failed")
    
    (*|_->raise(Failure "parsing failed")*)
  in 
  let(e, rest) = parse_lambda toks in
  match rest with
  |Lambda_EOF::[]->e
  |_ ->raise (Failure "parsing failed")*)
let parse_lambda toks = 
  let rec parse_lambda toks = 
    match toks with
    | Lambda_Var v :: rest -> (Var v, rest)
    | Lambda_LParen :: Lambda_Lambda :: Lambda_Var v :: Lambda_Dot :: rest ->
        let (e, rest') = parse_lambda rest in
        (match rest' with
        | Lambda_RParen :: rest'' -> (Func (v, e), rest'')
        | _ -> raise (Failure "parsing failed"))
    | Lambda_LParen :: rest ->
        let (e1, rest') = parse_lambda rest in 
        let (e2, rest'') = parse_lambda rest' in
        (match rest'' with
        | Lambda_RParen :: rest''' -> (Application (e1, e2), rest''')
        | _ -> raise (Failure "parsing failed"))
    | _ -> raise (Failure "parsing failed")
  in 
  let (e, rest) = parse_lambda toks in
  match rest with
  | Lambda_EOF :: [] -> e
  | _ -> raise (Failure "parsing failed")



let rec parse_c toks =
  match toks with  
    |Engl_If::rest->
      let (e1, rest1) = parse_c rest in
      (match rest1 with
      |Engl_Then::rest'->
        let(e2, rest'') = parse_c rest' in
          (match rest'' with
          |Engl_Else::rest'''->
            let(e3, rest'''') = parse_c rest''' in
            (If(e1, e2, e3), rest'''')
         |_->raise(Failure "parsing failed"))
      |_->raise(Failure "parsing failed"))
    |_ -> parse_H toks

and parse_H toks = 
  let(e1,rest) = parse_U toks in
  match rest with 
    |Engl_And::rest'->
    let(e2, rest'')= parse_H rest' in 
    (And(e1, e2),rest'')
    |Engl_Or::rest'->
    let(e3, rest''') = parse_H rest' in
    (Or(e1,e3),rest''')
    |_->(e1, rest)

and parse_U toks = 
  match toks with 
    |Engl_Not::rest->
    let(e1, rest') = parse_U rest in 
    (Not e1, rest')
    |_->parse_M toks


and parse_M toks = 
  match toks with
  |Engl_True::rest-> (Bool true, rest)
  |Engl_False::rest->(Bool false , rest)
  |Engl_LParen::rest->
    let (e1, rest')= parse_c rest in
    (match rest' with 
    |Engl_RParen::rest''->(e1,rest'')
    |_->raise(Failure "parsing failed"))
  |_->raise(Failure"parsing failed")


let rec c_string e =  
  match e with 
  |Bool b -> Printf.sprintf "Bool (%b)" b
  |Not n ->Printf.sprintf "Not (%s)" (c_string n)
  |And (left,right)->Printf.sprintf "And %s, %s" (c_string left) (c_string right)
  |Or (left,right)->Printf.sprintf "Or %s, %s" (c_string left) (c_string right)
  |If(cond, t , el)->Printf.sprintf "If %s then %s else %s" (c_string cond) (c_string t) (c_string el)
  
let token_string e = 
  match e with 
  |Engl_If -> "if"
  |Engl_Then -> "then"
  |Engl_Else -> "else"
  |Engl_And -> "and"
  |Engl_Or-> "or"
  |Engl_Not -> "not"
  |Engl_True-> "true"
  |Engl_False -> "false"
  |Engl_LParen -> "("
  |Engl_RParen -> ")"
  |Engl_EOF -> "end of string"


let parse_engl toks = 
  (*let rec parse_engl toks = *)
   (*expression and rest of the list. expression is the tree*)
    let (e, rest)= parse_c toks in
    Printf.printf "%s\n" (c_string e);
    Printf.printf ("%s\n") (String.concat " " (List.map token_string rest));
    if rest = [Engl_EOF] then e
    else 
    raise(Failure "left over tokens")
  (*in parse_engl []*)
  