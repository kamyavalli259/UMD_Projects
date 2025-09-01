open LccTypes 

let cntr = ref (-1)

let fresh () =
  cntr := !cntr + 1 ;
  !cntr

let reset () =
  cntr := (-1)


(*failwith ("Unimplemented; Optional")*)

let rec lookup env var = match env with 
  [] -> None
  |(v,e)::t -> if v = var then e else lookup t var

let rec subst expr var replacement = 
    match expr with
    |Var x -> if x = var then replacement else expr
    |Func(x, body1)->if x=var then expr 
    else
    let fresh_x =  fresh() in 
    let body' = subst(subst body1 x(Var (string_of_int fresh_x))) var replacement in
    Func((string_of_int fresh_x, body'))
    |Application(x1,y1)-> Application(subst x1 var replacement, subst y1 var replacement)
  
let rec alpha_convert e = 
  match e with
  |Var n -> Var n
  |Func (n, body)->
    let fresh_n = string_of_int (fresh ()) in
    Func (fresh_n, subst body n(Var fresh_n))
  |Application (p, k)->
    Application (alpha_convert p, alpha_convert k)
    

let isalpha e1 e2 = 
  let rec alpha e1 e2 = 
    match (e1, e2) with
    |(Var x, Var y)-> x = y
    |Func(x, body1), Func(y, body2)->
      let f = string_of_int (fresh()) in 
      (*alpha (subst body1 x(Var (string_of_int f))(subst body2 y(Var (string_of_int f))))*)
      alpha (subst body1 x(Var f)) (subst body2 y(Var f))
    |Application(x1,y1), Application(x2,y2)->
      alpha x1 x2 && alpha y1 y2
    |_->false
  in 
  alpha e1 e2
  
let rec eval env expr =
      match expr with
      | Var x -> (match lookup env x with
                  | Some e -> e
                  | None -> failwith "parsing failed")
      | Func (x, body) -> Func (x, body)
      | Application (e1, e2) ->
          let v1 = eval env e1 in
          let v2 = eval env e2 in
          (match v1 with
           | Func (x, body) -> eval ((x, Some v2) :: env) body
           | _ -> failwith "parsing failed")
(*the isapha can be Func("x", Var("x")) or Application(Func("y", Var("y")), Func("x", Var("x"))) *)
  (*match (e1,e2) with 
  |(Var(x), Var(x)) ->true
  |(Func(x,y), (Func(x,y))) ->true
  |(Application(x,x), Application(x,x))->true*)

(*|Var of var
  |Func of var * lambda_ast 
  |Application of lambda_ast* lambda_ast*)


let rec reduce env e = 
  match e with 
  |Var x -> 
    (match lookup env x with 
    |Some v-> v
    |None->e
    |_->e)
  |Func(x,body)->Func(x, body)
  |Application(Func(v, body), e2)->
    let e2' = reduce env e2 in
    let body'= subst body v e2' in
    reduce env body'
  |Application(x1,y1)->
    let x1'= reduce env x1 in 
    let y1' = reduce env y1 in
    (match x1' with
    |Func(z,body)->reduce env (subst body z y1')
  |_->Application(x1',y1'))
  |_->raise (Failure "parsing failed")

let rec convert tree = 
  match tree with 
  |Bool true -> "(Lx.(Ly.x))"
  |Bool false -> "(Lx.(Ly.y))"
  |If (a, b, c)->
    let a' = convert a in 
    let b' = convert b in 
    let c' = convert c in 
    Printf.sprintf "((%s %s) %s)" a' b' c'
  |And (a, b)->
    let a' = convert a in
    let b' = convert b in 
    Printf.sprintf "(((Lx.(Ly.((x y) (Lx.(Ly.y))))) %s) %s)" a' b'
  |Not a->
    let a' = convert a in
    Printf.sprintf "((Lx.((x (Lx.(Ly.y))) (Lx.(Ly.x)))) %s)" a'
  |Or (a, b)->
    let a' = convert a in
    let b' = convert b in 
    Printf.sprintf "(((Lx.(Ly.((x (Lx.(Ly.x))) y))) %s) %s)" a' b'
  |_->raise(Failure "parsing failed")


(*let rec readable tree = 
  match tree with 
  |Var v -> v
  |Func (var1, Func (var2, Var var3)) -> 
    if var1 = var3 then "true" else "false"
  
  |Application(Application(Func (var1, Func (var2, Application(Application(Var v3, Var v4), Func(v5, Func (v6, Var n_var))))), arg1), arg2)->
    Printf.sprintf "(%s and %s)" (readable arg1) (readable arg2)
  |Application(Application(Func(var1, Func(var2, Application(Application(Var var3, Func(v4, Func(v5, Var v6))), Var v7))), arg1), arg2)->
    Printf.sprintf "(%s or %s)" (readable arg1) (readable arg2)
  |Application(Func (var1, Application(Application (Var v2, Func(v3, Func(v4, Var v5))), Func(v6, Func(v7, Var v8)))), arg1)->
    Printf.sprintf "(not %s)" (readable arg1)
  |Application (Application (cond, t), e) -> 
    Printf.sprintf "(if %s then %s else %s)" (readable cond)(readable t)(readable e) 
  |Application (f, x) ->
    let readable_f = match f with
      | Application _ -> Printf.sprintf "(%s)" (readable f)
      | _ -> readable f
    in
    let readable_x = match x with
      | Application _ -> Printf.sprintf "(%s)" (readable x)
      | _ -> readable x
    in
    Printf.sprintf "%s %s" readable_f readable_x
  
  |_->raise(Failure "parsing failed")*)
let rec readable tree = match tree with
  | Application(Func(var1,Application(Application(var2,Func(v3,Func(v4,v5))),Func(v6,Func(v7,v8)))),e) ->
    "(not " ^ (readable e) ^ ")"
  | Application(Application(Func(v1,Func(v2,Application(Application(v3,v4),Func(v5,Func(v6,v7))))),a),b) ->
    "(" ^ (readable a) ^ " and " ^ (readable b) ^ ")"
  | Application(Application(Func(v1,Func(v2,Application(Application(Var(v3),Func(v4,Func(v5,Var(v6)))),Var(v7)))),a),b)->
    "(" ^ (readable a) ^ " or " ^ (readable b) ^ ")"
  | Application(Application(s1,s2),s3) -> "(if " ^ (readable s1) ^
      " then " ^ (readable s2) ^ " else " ^ (readable s3) ^ ")"
  | t ->
    let true_tree = Func("x",Func("y",Var("x"))) in
    if (isalpha t true_tree) then "true" else "false"
  |_->raise(Failure "parsing failed")
  


(*and string_of_expr expr =
  match expr with
  | Var v -> Printf.sprintf "Var %s" v
  | Func (v, body) -> Printf.sprintf "Func (%s, %s)" v (string_of_expr body)
  | Application (e1, e2) -> Printf.sprintf "Application (%s, %s)" (string_of_expr e1) (string_of_expr e2)*)
  



