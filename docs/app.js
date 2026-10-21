const $ = id => document.getElementById(id);
const source = $("source");
const keywordTypes = new Set(["int","float","void","return"]);
const example = source.value;
$("resetBtn").addEventListener("click", () => { source.value = example; updateLines(); });
source.addEventListener("input", updateLines);
function updateLines(){ $("lineNumbers").innerHTML = source.value.split("\n").map((_,i)=>i+1).join("<br>"); }
function lex(code){
  const re = /\s+|[A-Za-z_][A-Za-z0-9_]*|\d+(?:\.\d+)?|[(){};,=+\-*/]|./gy;
  const tokens=[]; let m,line=1,col=1;
  while((m=re.exec(code))!==null){
    const v=m[0], l=line,c=col;
    if(/^\s+$/.test(v)){ for(const ch of v){if(ch==="\n"){line++;col=1}else col++;} continue; }
    let type;
    if(keywordTypes.has(v)) type=v.toUpperCase();
    else if(/^[A-Za-z_]/.test(v)) type="IDENTIFIER";
    else if(/^\d/.test(v)) type="NUMBER";
    else type=({"(":"LPAREN",")":"RPAREN","{":"LBRACE","}":"RBRACE",",":"COMMA",";":"SEMICOLON","+":"PLUS","-":"MINUS","*":"STAR","/":"SLASH","=":"ASSIGN"})[v]||"INVALID";
    tokens.push({type,value:v,line:l,col:c});
    col+=v.length;
    if(type==="INVALID") throw new Error(`Invalid character '${v}' at line ${l}, column ${c}.`);
  }
  tokens.push({type:"EOF",value:"",line,col}); return tokens;
}
function parse(tokens){
  let i=0; const peek=()=>tokens[i]; const take=()=>tokens[i++]; const need=(type,msg)=>{if(peek().type!==type)throw new Error(`${msg} at line ${peek().line}; found '${peek().value||"end of input"}'.`);return take();};
  const type=()=>{if(["INT","FLOAT","VOID"].includes(peek().type))return take().value;throw new Error(`Expected a type at line ${peek().line}.`);};
  const primary=()=>{if(peek().type==="NUMBER"||peek().type==="IDENTIFIER")return take();if(peek().type==="LPAREN"){take();const x=expr();need("RPAREN","Expected ')'");return x;}throw new Error(`Expected number or identifier at line ${peek().line}.`);};
  const expr=()=>{let a=primary();while(["PLUS","MINUS","STAR","SLASH"].includes(peek().type)){const op=take();const b=primary();a={type:"binary",op:op.value,left:a,right:b};}return a;};
  const functions=[];
  while(peek().type!=="EOF"){
    const returnType=type(), name=need("IDENTIFIER","Expected function name").value;
    need("LPAREN","Expected '(' after function name");const params=[];
    if(peek().type!=="RPAREN"){do{const t=type(),n=need("IDENTIFIER","Expected parameter name").value;params.push({type:t,name:n});if(peek().type!=="COMMA")break;take();}while(true);}
    need("RPAREN","Expected ')' after parameters");need("LBRACE","Expected '{' to begin function body");const statements=[];
    while(!["RBRACE","EOF"].includes(peek().type)){
      if(peek().type==="RETURN"){take();if(peek().type==="SEMICOLON"){take();statements.push({kind:"return-empty"});}else{const e=expr();need("SEMICOLON","Expected ';' after return expression");statements.push({kind:"return",expr:e});}}
      else if(["INT","FLOAT"].includes(peek().type)){const t=type(),n=need("IDENTIFIER","Expected variable name").value;need("SEMICOLON","Expected ';' after declaration");statements.push({kind:"decl",type:t,name:n});}
      else throw new Error(`Expected return statement or declaration at line ${peek().line}.`);
    }
    need("RBRACE","Expected '}' at end of function");functions.push({returnType,name,params,statements});
  }
  return functions;
}
function semantic(functions){
  const errors=[],names=new Set();
  const idsIn=(e,out=[])=>{if(!e)return out;if(e.type==="IDENTIFIER")out.push(e.value);if(e.left)idsIn(e.left,out);if(e.right)idsIn(e.right,out);return out;};
  functions.forEach(f=>{
    if(names.has(f.name))errors.push(`Duplicate function name '${f.name}'.`);names.add(f.name);
    const scope=new Set();
    f.params.forEach(p=>{if(scope.has(p.name))errors.push(`Function '${f.name}' has duplicate parameter '${p.name}'.`);scope.add(p.name);});
    f.statements.filter(s=>s.kind==="decl").forEach(s=>scope.add(s.name));
    f.statements.forEach(s=>{
      if(f.returnType==="void"&&s.kind==="return")errors.push(`Void function '${f.name}' should use 'return;' without a value.`);
      if(s.kind==="return")idsIn(s.expr).forEach(id=>{if(!scope.has(id))errors.push(`Function '${f.name}' uses undeclared identifier '${id}'.`);});
    });
  });
  return errors;
}
function renderTokens(tokens){
  $("tokenCount").textContent=`${tokens.length-1} tokens`;
  $("tokens").innerHTML=tokens.filter(t=>t.type!=="EOF").map(t=>`<div class="token-row"><span>${escapeHtml(t.type)}</span><b>${escapeHtml(t.value)}</b><small>${t.line}:${t.col}</small></div>`).join("")||'<p class="empty">No tokens found.</p>';
}
function escapeHtml(s){return String(s).replace(/[&<>"']/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));}
function setState(id,text,ok){$(id).textContent=text;$(id).style.color=ok?"#416b50":"#b66b42";}
function compile(){
  $("status").innerHTML="<span></span> Compiling…";
  try{
    const tokens=lex(source.value);renderTokens(tokens);setState("lexState","Passed",true);
    const functions=parse(tokens);setState("parseState","Passed",true);
    const errors=semantic(functions);setState("semanticState",errors.length?`${errors.length} issue(s)`:"Passed",!errors.length);
    $("functionCount").textContent=`${functions.length} function${functions.length===1?"":"s"}`;
    $("functions").innerHTML=functions.map(f=>`<div class="function-item"><b>${escapeHtml(f.returnType)} ${escapeHtml(f.name)}(${f.params.map(p=>escapeHtml(p.type+" "+p.name)).join(", ")})</b><p>${f.statements.length} statement(s) · ${f.params.length} parameter(s)</p></div>`).join("")||'<p class="empty">No functions found.</p>';
    const report=$("report");
    if(errors.length){report.className="report error";report.innerHTML=`<div class="report-icon">!</div><b>Syntax passed, semantic checks found ${errors.length} issue(s)</b><p>${errors.map(escapeHtml).join("<br>")}</p>`;$("status").innerHTML='<span style="background:#d58a57"></span> Issues found';}
    else{report.className="report success";report.innerHTML=`<div class="report-icon">✓</div><b>Compilation checks passed</b><p>${functions.length} function definition(s) parsed. Lexer, parser, and basic semantic checks succeeded.</p>`;$("status").innerHTML='<span></span> Checks passed';}
  }catch(err){
    setState("parseState","Needs fix",false);setState("semanticState","Not run",false);
    const report=$("report");report.className="report error";report.innerHTML=`<div class="report-icon">!</div><b>Compilation stopped</b><p>${escapeHtml(err.message)}</p>`;
    $("status").innerHTML='<span style="background:#d58a57"></span> Fix source errors';
  }
}
$("compileBtn").addEventListener("click",compile);
updateLines();compile();
