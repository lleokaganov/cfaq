var zindexstart=100; // начало отсчета слоев для окон

WWWAJAX='/';
// WWWAJAX='/ESP8266/AJ.php';

//========================================================


plays=function(a,silent,menu){
    if(menu) return helpc('play','play',"<audio src='"+a+"'"+(silent?'':" autoplay='autoplay'")+" controls></audio>");
    mkdiv('plays',"<div style='position:absolute;width:1px;height:1px;overflow:hidden;left:-40px;top:0;opacity:0'><audio"+(silent?'':" autoplay='autoplay'")+" src='"+a+"'></audio></div>");
    otkryl(id);
};

function c_save(n,v) { var N=new Date(); N.setTime(N.getTime()+(v==''?-1:3153600000000)); document.cookie=n+'='+encodeURIComponent(v)+';expires='+N.toGMTString()+';path=/;'; }

function f5_save(k,v){ try { var w=window.localStorage; return k.length<500&&v.length<20000&&w&&w.setItem?w.setItem(k,v):false; } catch(e){} }
function f5_read(k){ try { var w=window.localStorage; return w&&w.getItem?w.getItem(k):false; } catch(e){} }

function idd(i){ return typeof(i)=='object'?i:typeof(document.getElementById(i))=='undefined'?false:document.getElementById(i); }
function zabil(i,s) { i=idd(i); if(i) i.innerHTML=s; }

function doclass(cla,f,s,node) { var p=getElementsByClass(cla,node?node:document);
    for(var i in p) { if(typeof(p[i])!='undefined' && typeof(p[i].className)!='undefined') f(p[i],s); }
}

function vzyal(i) { i=idd(i); return i?i.innerHTML:''; }
function zakryl(i) { i=idd(i); if(i) i.style.display='none'; }
function otkryl(i) { i=idd(i); if(i) i.style.display='block'; }

var print_r_id=0;
var print_rid={};
function printr_f(ev,e,i){ ev.stopPropagation();
    if(e.className!='ll') { e.innerHTML="[Object]"; e.className='ll'; return; }
    e.className=''; e.style.marginLeft='30px'; e.innerHTML='{\n'+print_r(print_rid[i],0,1)+'\n}\n';
}
function print_r(a,n,skoka) {
    var s='',t='',i,v; if(!n)n=0; for(i=0;i<n*10;i++)t=t+' ';
    if(typeof(a)!='object') return a;

    for(var j in a){ if(typeof(j)=='undefined' || typeof(a[j])=='undefined') break;
	v=a[j]; if(v!=null && !skoka && typeof(v)=='object' && typeof(v.innerHTML)!='string') v=print_r(v,n+1);
	if(v=='[object Object]') {
	    var z=(print_r_id++);
	    print_rid[z]=Object.assign({},v);
	    s='\n'+t+j+'='+"<div onclick=\"printr_f(event,this,'"+z+"')\" class=ll>[Object] {"+z+"}</div>" +s;
	} else s='\n'+t+j+'='+v+s;
    }
    return s;
}

function in_array(s,a){ for(var l in a) if(a[l]==s) return l; return false; }

function clean(i) { i=idd(i); setTimeout(function(){if(i)i.parentNode.removeChild(i)},10); }

if(document.getElementsByClassName) getElementsByClass=function(c,node){ return (node||document).getElementsByClassName(c) };
else {
    getElementsByClass = function(classList, node) {
        var node = node || document, list = node.getElementsByTagName('*'),
        length = list.length, classArray = classList.split(/\s+/),
        classes = classArray.length, result = [], i,j;
        for(i = 0; i < length; i++) {
            for(j = 0; j < classes; j++) {
                if(list[i].className.search('\\b' + classArray[j] + '\\b') != -1) { // alert(1);
                    result.push(list[i]);
                    break;
                }
            }
        }
        return result;
    };
}

// создать новый <DIV class='cls' id='id'>s</div> в элементе paren (если не указан - то просто в документе)
// есть указан relative - то следующим за relative, иначе - просто последним
function mkdiv(id,s,cls,paren,relative){ if(idd(id)) { zabil(id,s); idd(id).className=cls; return; }
    var div=document.createElement('DIV'); div.className=cls; div.id=id; div.innerHTML=s; div.style.display='none';
    if(paren==undefined) paren=document.body;
    if(relative==undefined) paren.appendChild(div);
    else paren.insertBefore(div,relative.nextSibling);
}
function newdiv(s,ara,paren,relative){ if(typeof(ara)!='object') ara={}; mkdiv(ara.id,s,ara.cls,paren,relative,1); }

function posdiv(id,x,y) { // позиционирование с проверкой на вылет, если аргумент '-1' - по центру экрана
    var e=idd(id),DW,DH;
    if(e.className=='newin') e=idd(id+'_body');
    e.style.position='absolute';
    if(e.style.display!='block') otkryl(id);
    var W=getWinW(),H=getWinH(),w=e.clientWidth,h=e.clientHeight;
    if(x==-1) x=(W-w)/2+getScrollW();
    if(y==-1) y=(H-h)/2+getScrollH();
    DW=W-10; if(w<DW && x+w>DW) x=DW-w; if(x<0) x=0;
    if(y<0) y=0;
    if(idd(id).className!='newin') e.style.top=y+'px';
    e.style.left=x+'px';
    otkryl(id);
}

function center(id) { posdiv(id,-1,-1); }

function addEvent(e,evType,fn) {
    if(e.addEventListener) { e.addEventListener(evType,fn,false); return true; }
    if(e.attachEvent) { var r = e.attachEvent('on' + evType, fn); return r; }
    e['on' + evType] = fn;
}

function removeEvent(e,evType,fn){
    if(e.removeEventListener) { e.removeEventListener(evType,fn,false); return true; }
    if(e.detachEvent) { e.detachEvent('on'+evType, fn) };
}

function helpc(id,z,s) { helps(id,mk_helpbody(z,s)); }
function mk_helpbody(z,s) { return "<fieldset>"+(z==''?'':"<legend>"+z+"</legend>")+"<div class='textbody'>"+s+"</div></fieldset>"; }
function idie(s,t) { var e=typeof(s); if(e=='object') s="<pre style='max-width:"+(getWinW()-200)+"px'>"+print_r(s,0,1)+'</pre>';
if(t!=undefined) s=t+'<p>'+s;
var p=idd('idie'); if(p) { p=getElementsByClass('textbody',p)[0]; if(p) return p.innerHTML=p.innerHTML+'<hr>'+s; }
helpc('idie','Error type: '+e,s) }
dier=idie;

function helps(id,s,pos) {

if(!idd(id)) {

// &#128308; &#127377;
// mkdiv(id,"<div id='"+id+"_body'>"+s+"</div><i id='"+id+"_close' onclick=\"clean('"+id+"')\" title='Close' class='can'></i>",'pop2');
mkdiv(id,"<div id='"+id+"_body'>"+s+"</div><div class='us in mv' style='font-size:32px; margin:0; padding:0; position:absolute; right:-19px; top:-19px; cursor:pointer;' onclick=\"clean('"+id+"')\" \
title='Close'>&#11093;</div>",'pop2');

var e_body=idd(id+'_body'); // За тело не таскаем
var hmov=false; // Предыдущие координаты мыши
var e=idd(id);
var pnt=e; while(pnt.parentNode) pnt=pnt.parentNode; //Ищем Адама

var mmFunc=function(ev) { ev=ev||window.event; if(hmov) {
	e.style.left = parseFloat(e.style.left)+ev.clientX-hmov.x+'px';
	e.style.top = parseFloat(e.style.top)+ev.clientY-hmov.y+'px';
	hmov={ x:ev.clientX, y:ev.clientY };
	if(ev.preventDefault) ev.preventDefault();
	return false;
    }
};

var muFunc=function(){ if(hmov){
    hmov=false;
    removeEvent(pnt,'mousemove',mmFunc);
    removeEvent(pnt,'mouseup',muFunc);
    e.style.cursor='auto';
    }
};

addEvent(e,'mousedown', function(ev){ if(hmov) return;

ev=ev||window.event;
var lbtn=(window.addEventListener?0:1); //Если ИЕ, левая кнопка=1, иначе 0
if(!ev.target) ev.target=ev.srcElement;
if((lbtn!==ev.button)) return; //Это была не левая кнопка или 'тело' окна, ничего не делаем
var tgt=ev.target;
while(tgt){
    if(tgt==e_body) return;
    if(tgt==e) break;
    tgt=tgt.parentNode;
};
//Начинаем перетаскивать
e.style.cursor='move';
// hmov2=0;
hmov={ x:ev.clientX, y:ev.clientY };
addEvent(pnt,'mousemove',mmFunc);
addEvent(pnt,'mouseup',muFunc);
if(ev.preventDefault) ev.preventDefault();
return false;
});
// ===========================================================================

} else { zabil(id+'_body',s); }

idd(id).style.zIndex=(zindexstart++);
center(id);
}


function getScrollH(){ return document.documentElement.scrollTop || document.body.scrollTop; }
function getScrollW(){ return document.documentElement.scrollLeft || document.body.scrollLeft; }
function getWinW(){ return window.innerWidth || (document.compatMode=='CSS1Compat' && !window.opera ? document.documentElement.clientWidth : document.body.clientWidth); }
function getWinH(){ return window.innerHeight || (document.compatMode=='CSS1Compat' && !window.opera ? document.documentElement.clientHeight : document.body.clientHeight); }

function salert(l,t) {
var p=idd('salert'); if(p){ p=getElementsByClass('textbody',p)[0]; if(p) {p.innerHTML=p.innerHTML+'<hr>'+l; return false; } }
helps('salert',"<div style='padding:20px' class='textbody'>"+l+"</div>"); if(t) setTimeout("clean('salert')",t); return false; }

//-------------------------------------------------------------------------
function h(s){ return s.replace(/\&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/\'/g,'&#039;').replace(/\"/g,'&#034;'); } // '
function c(s) { return s.replace(/^\s+/g,'').replace(/\s+$/g,''); }

// *********************** majax **********************
function ajaxon(){ var id='ajaxgif'; mkdiv(id,"<img src=ajaxm.gif>",'popup'); center(id); }
function ajaxoff(){ clean('ajaxgif'); }

// function MO(s,f) { alert("=== MO: \n\n"+s); AJAX('/MOTO?'+encodeURIComponent(s),{noajax:1,callback:(f?f:function(s){})}); }
function AJ(s,f) { AJAX(WWWAJAX,{noajax:1,callback:(f?f:function(s){})},s); }

function AJAX(url,opt,s) { if(!opt) opt={};
    if(!opt.noajax) ajaxon();
    var xhr=new XMLHttpRequest();
    if(opt.error) xhr.onerror=opt.error;
    xhr.timeout=(opt.timeout ? opt.timeout : 2000);
    if(opt.ontimeout) xhr.ontimeout=opt.ontimeout;

    xhr.onreadystatechange=function(){
	if(this.readyState==4 && this.status==200 && this.responseText!=null) {
          // try{
	        if(!opt.noajax) ajaxoff();
		if(opt.callback) opt.callback(this.responseText,url,s);
		else eval(this.responseText);
          // }catch(e){alert('Error Ajax: '+this.responseText);}
	} else if(this.status=="404") { if(!opt.noajax) ajaxoff(); if(opt.error) opt.error(); else salert(url+"<br>404 not found",1000); }
	// else { alert('----------> ErrorStatus='+this.readyState+" / "+this.status); }
    };
    xhr.open((opt.method?opt.method:(s?'POST':'GET')),url,(opt.async?opt.async:true));
    if(s) xhr.send(s); else xhr.send();
}


//------------- /FM ----------

// function DigitalRead(id,d) { AJAX("/AJAX?a=read&d="+d,{ callback: function(res){ zabil(id,res); }, noajax:1 }); }
// function AnalogRead(id,d) { AJAX("/AJAX?a=aread",{ callback: function(res){ zabil(id,res); }, noajax:1 }); }

function WIFIconnp(net) { helpc('wifipass','&#128274; password for '+net,"<input type=password size=10 onchange=\"WIFIconn('"+h(net)+"',this.value)\">"); }
function WIFIconn(net,pass) {
    helpc('wificonn','Connecting to '+net,"Wait... <img src=ajaxm.gif>");

var s="loop 1 0\n\
loop 2 0\n\
loop 3 0\n\
WIFI.disconnect YES\n\
WIFI.APdisconnect YES\n\
usleep 500\n\
\n\
WIFI.autoconnect YES\n\
WIFI.autoreconnect YES\n\
WIFI.persistent FALSE\n\
WIFI.mode STA\n\
WIFI.dns NO\n\
\n\
WIFI "+net+" "+pass+"\n\
WIFI.waitconnect\n\
\n\
if.!WIFI {\n\
    echo ERROR\n\
    exit\n\
}\n\
\n\
echo SUCCESS\\nhttp://{ip}\\nhttp://{mdns}.local\n\
playip\n\
\n\
FILE.save.text /wifi_last.txt "+net+"\\n"+pass+"\n\
\n\
ping http://canada.lleo.me/bot/t.php?id={chip}-{FlashChipId}&soft={soft}&say=РЎС‚Р°РЅС†РёСЏ%20{chip}%20СЃРѕРµРґРёРЅРёР»Р°СЃСЊ%20СЃ%20WIFI%20{ip_ssid}&ip={ip}\n\
";

// stoplist.txt - список файлов, запрещенных для автообновления с сервера-матки (автоматически вносятся в список при онлайн-редактировании)
// stopweblist.txt - список файлов, запрещенных для показа с веба НЕ АДМИНУ
// web_deprecate.txt - список файлов, которые задолбали спрашивать, мгновенный отлуп

//  AJAX("/FM?a=WIFIconn&net="+encodeURIComponent(net)+"&pass="+(pass?encodeURIComponent(pass):''),{callback:function(s){alert(s);}} );

AJ(s,function(x){alert(x)});

}

function fm(e) { AJAX('FM?a='+e.value) }

function buf(e) { AJAX('/',{},e) }

function idd(x) { return document.getElementById(x) }

function ibu(e) {
    idd(e.id+'d').innerHTML=e.value;
    AJAX('/MOTO?file=go+'+e.id.replace(/servo/g,'')+'+'+e.value);
}

ffile=function(e){ return e.closest("TR").id.replace(/^file_/g,''); };

fdel=function(e){ e=ffile(e); if(confirm("Delete "+e+"?")) AJ("FILE.add.string /stoplist.txt "+e+"\n"+"FILE.del "+e,function(s){clean('file_'+e);salert(s,500);}); };

fupload=function(e){ AJAX("/FM?a=upload&file="+encodeURIComponent( typeof(e)=="string"?e:ffile(e) ) ); };

fedit=function(e,newfile){
    var file=(typeof(e)=="string"?e:ffile(e));

    var F=file.toUpperCase();
    var im=("JPG JPEG PNG GIF ICO SVG BMP WEBP").split(' '); for(var i in im) if(F.endsWith("."+im[i])) return idie("image","<img src='"+h(file)+"'>");
    var im=("MP3 OGG").split(' '); for(var i in im) if(F.endsWith("."+im[i])) return plays(file,0,1);

    helps('editor',"<div><b>"+h(file)+"</b> &nbsp; <span onclick=\"delit('"+h(file)+"')\">&#128274;</span></div>\
<form style='margin:0 0 0 0' id='edit_form'><input type='hidden' name='file' value='"+h(file)+"'>\
<text"+"area style='margin:0 0 0 0;padding:0 0 0 0;width:"+Math.floor(getWinW()*0.90)+"px;height:"+Math.floor(getWinH()*0.80)+"px;' name='text' id='ta_edit'>{s}</text"+"area>\
<br><input type='button' value='Save' onclick='savea_edi(this,\""+file+"\",0,"+newfile+")'> &nbsp; <input type='button' value='Save no close' onclick='savea_edi(this,\""+file+"\",1,"+newfile+")'>\
</form>");

    if(!newfile) AJAX(file,{ callback: function(s){ idd('ta_edit').value=s; } });
};

function savea_edi(e,file,noclose,newfile){
    var b=String(Math.random()).slice(2);
    var x=new XMLHttpRequest();
    x.open('POST','/upload',true);
    x.setRequestHeader('Content-Type','multipart/form-data; boundary='+b);
    x.onreadystatechange=function() { if(this.readyState != 4) return;
	ajaxoff();
	if(this.status!=200 || this.responseText==null) return idie("Save Error");
	if(this.responseText != "OK") return idie("ERROR: "+this.responseText);
	if(!noclose) clean('editor');
	if(newfile) { alert('new'); }
	salert('saved',300);
	    var o="FILE.string.add /stoplist.txt "+file+"\n"+"echo {FILE:/stoplist.txt}";
	    if(file=="/admin_index.htm") o+="\n"+"FILE.key.del /Settings.txt WebPage";
	    AJ(o,function(s){stoplist=s.split("\n")});
	return;
    };
    x.send('--'+b+'\r\n'+'Content-Disposition: form-data; name="'+h(file)+'"; filename="'+h(file)+'"\r\n\r\n'+e.form.text.value+'\r\n--'+b+'--\r\n');
    ajaxon();
}

// function savea_edit(e,noclose){ AJAX("/FM?a=editsave&noclose="+(noclose?1:0)+"&file="+encodeURI(e.form.file.value),{},e.form.text.value); }

function FM_info() { AJAX("/info",{callback:function(s){ helpc('info','info',toTable("Full Info",s));}}); }
function FM_firmware() { AJAX("/FM?a=upgrade&file=firmware",{callback:function(s){ alert(s); }}); }

function toTable(M,s) {
 s=s.replace(/^\n+/,'').replace(/\n+$/,'');
 s=s.replace(/\n(==[^\n]+)/g,"</td></tr><tr><td colspan=2 align=center><b>$1</b>");
 s=s.replace(/\n+/g,"</td></tr><tr><td>");
 s=s.replace(/\: /g,"</td><td align=left>");
 return "<table><tr><td colspan=2 align=center><H2>"+M+"</h2></td></tr><tr><td>"+s+"</td></tr></table>";
}


function nostop(e) { var file=ffile(e);
    AJ( (e.innerHTML=='&nbsp;'?"FILE.string.add":"FILE.string.del")+" /stoplist.txt "+file+"\necho {FILE:/stoplist.txt}", function(s){ stoplist=s.split("\n") });
    e.innerHTML=(e.innerHTML=='&nbsp;'?"&#128274;":"&nbsp;");
}

var stoplist=[];

function FM_fileman() {
    AJ("echo {dir}<===>{FILE:/stoplist.txt}",function(s){
	if(s=='') return idie("no files");
	s=s.split("<===>"); stoplist=s[1].split("\n"); s=s[0];
	s=s.split("\n");
	// СЃРѕСЂС‚РёСЂРѕРІРєР°
	var ss={}; var sk=[]; for(var i in s) { if(typeof(s[i])!='string' || s[i]=='') continue; var l=s[i].split(' '); ss[l[1]] = l[0]; sk.push(l[1]);  }
	sk.sort();
	s=[]; for(var i in sk) { if(typeof(sk[i])!='string' || sk[i]=='') continue; s.push(ss[sk[i]]+' '+sk[i]); }

	var o="";
	for(i=0;i<s.length;i++) { var c=s[i]; if(c=='') continue;
	    var n=c.split(' ')[0],l=c.substring(n.length+1);
	    // o+="size: ["+n+"] file: ["+l+"]\n"; //  &nbsp; <i onclick='fupload(this)' class='mv e_upload'></i>\
	    //  &nbsp; <i onclick='fedit(this)' class='mv e_edit'></i> &nbsp; 
	    o+="<tr id=\"file_"+l+"\">\
<td onclick='fdel(this)' class='mv'>&#10060; &#160; </td>\
<td style='cursor:pointer;color:blue;' onclick='fedit(this)'>"+l+"</td>\
<td> &#160; &#160; </td>\
<td alt='Change dostup' onclick='nostop(this)' class='mv us'>"+(in_array(l,stoplist)?'&#128274;':'&nbsp;')+"</td>\
<td style='font-size:8px;'><span id=\"size_"+i+"\">"+n+"</span> bytes</td>\
</tr>";
	}

o="<table border=0 id='filetab'>"+o+"</table>";

o+="<div class=br><i onclick=\"fedit(idd('newfileid').value,1)\" class='mv e_edit'></i> &nbsp; <i onclick=\"fupload(idd('newfileid').value)\" class='e_upload'></i>\
 &nbsp; <input type=text value='' size=10 id='newfileid'></div>\
<div class='br'><i onclick='FM_firmware()' class='e_ledgreen'></i></div>\
<div><input class='br' type='button' value='firmware.bin' onclick='FM_firmware()'></div>";

    helpc('files_edit',"Files Editor",o);
});
}

function logingo(p) { AJAX("/FM?a=login&password="+encodeURI(p),{
	    callback: function(x){ c_save('ESP',x);
	    // clean('login');
	    if(x=="") return salert("Error password",2000); window.location.reload(true); },
	    noajax:1
	});
}

function login() { helpc('login','login',"Password: <input type=password onchange='logingo(this.value)'>"); }
function logout() { if(confirm('Logout?')) { c_save('ESP',"logout"); window.location.reload(true); } }

// function dbm2prc(dBm) { return (1*dBm <= -100 ? 0 : (1*dBm >= -50 ? 100 : 2*(1*dBm + 100) )); }

function wifiscan(e) {
    helpc('wifiscan','WIFI scan',"Scanning...<img src='/ajaxm.gif'>");
    AJAX("/",{timeout:15000,noajax:1,callback: function(s){wifi_print(s)}},"echo {WIFI.scan}");
}











function exewf(s) {
    var net=idd('wftb').querySelector("DIV[class='br slc']").closest("TR").querySelector("TD[class='ll']").innerHTML;
    s=s.replace(/\{thisnet\}/g,net);
    alert(s);
}

function tareawf(e) { e=e.innerHTML; var s='';
    if(e=='[pass]') s="\
at 10 WIFI MYNET MYPASS\n\
WIFI {thisnet}\n\
WIFI.waitconnect\n\
}";
    else if(e=='[id]') s="\
at 10 WIFI MYNET MYPASS\n\
WIFI {thisnet}\n\
WIFI.waitconnect\n\
}";
    else return;
    idd('tareawf').value=s;
}

function wfcl(e) {
    var p=idd('wftb').querySelectorAll("DIV[class='br slc']");
    for(var i=0;i<p.length;i++) { p[i].style.border='none'; p[i].className='br'; }
    e.style.border='2px solid green'; e.className='br slc';

    if(vzyal('wfdo').indexOf('<hr>')>=0) {
	var o="\
<center><div class='divta'>\
<span class='ri'>\
<span class='mv' onclick=\"exewf(this.closest('DIV').querySelector('TEXTAREA').value)\">WIFI</span></span>\
<textarea rows=5 class='tat' id='tareawf' onchange=\"f5_save(this.id,this.value)\"></textarea>\
\
<div class='br ll' onclick='tareawf(this)'>[pass]</div> \
<div class='br ll' onclick='tareawf(this)'>[id]</div> \
\
</div></center>";
	zabil('wfdo',o);
	var i='tareawf',s=f5_read(i); if(s) idd(i).value=s;
    }
}

function wifi_print(s) {
    var p=s.split("\n");
    var s="";
    var channels=[0,0,0,0,0,0,0,0,0,0,0];
    for(var i in p) { if(p[i].length==0) continue;
        var d=p[i].split(" ");
        s+="<tr>\
<td>"+(1*d[0]?'&#128274;':'&#128275;')+"</td>\
<td>"+d[1]+"</td>\
<td>"+d[2]+"</td>\
<td>"+d[3]+"</td>\
<td>"+d[4]+"%</td>\
<td><div style='width:"+(d[4]/2)+"px;height:10px;background-color:red'></div></td>\
<td><div class='br' onclick='wfcl(this)'>"+d[5]+"</div></td>\
<td class=ll onclick='WIFIconn"+(d[0]?'p':'')+"(this.innerHTML)'>"+d.slice(6).join(' ')+"</td>\
</tr>\n";
	channels[d[1]-1]+=1*d[4];
    }
    var o='';
    for(var i=0;i<11;i++) o+="<div class='br'>"+(i+1)+" <div class='in' style='width:"+(channels[i])+"px;height:20px;background-color:green'></div></div>";
    helpc('wifiscan','WIFI scan',"<table id='wftb'>"+s+"</table>" + "<div id='wfdo'><hr>"+o+"</div>");
}

// ====

function ESPrestart() { if(!confirm('Restart ESP?')) return;
    AJ("play DRMFSL\nESP.restart");
    document.body.innerHTML="<img src='ajaxm.gif' style='width:10%;height:10%;margin:10%;'>";
    setTimeout('document.location.reload(true)',3000);
}









function WebPageStart(tmp,defki) {

 if(tmp) {
    var p=tmp.split("\n");for(var i in p) p[i]=c(p[i]);
    var o=''; for(var i in p) {
        if(p[i].indexOf('|')<0) continue; //>
        var u=p[i].split('|'); for(var i in u) u[i]=c(u[i]);
        var typ=u[0];
        var varid=u[1];
        var def=u[2];
        var name=u[3];
        var comment=u[4];
        var ink='['+typ+']';
        if(typ=='TEXTAREA') ink="<text"+"area class='"+h(varid)+"' name='"+h(varid)+"' rows='3' cols='40' placeholder=\""+h(def.replace(/ /g,"\n"))+"\"><"+"/text"+"area>";
        else if(typ=='INPUT') ink="<input size='15' class='"+h(varid)+"' name='"+h(varid)+"' type='text' placeholder=\""+h(def)+"\" value=''>";
        o+="<tr><td align='left'><b>"+name+"</b><div class=br>"+comment+"</div></td><td align='right'>&nbsp;"+ink+"</td></tr>";
    }
    o+="<tr><td colspan=2><input class='br' type='submit' value='Save' onclick='ChangeSettings(this); return false;'></td></tr>";
    zabil('settings',o);
 }

    AJAX("/Settings.txt",{noajax:1,callback:function(s){ s=s.split("\n");
        var pp={}; for(var i in s) {
            var u=c(s[i]).replace(/\#.*$\s+/g,'');
            if(u.indexOf(' ')<1) continue;// >
            var name=u.split(' ')[0];
            var value=u.substring(name.length+1);
            pp[name]=value;

	    if(name=="DeviceName" && value.indexOf('Default') != -1) zabil('settings',"<tr><td align='left'><b>"+h(name)+"</b></td><td align='right'>&nbsp;<input size='15' name='"+h(name)+"' type='text' value='"+h(value)+"'></td></tr>"+vzyal('settings'));

            doclass(name,function(e,v){
                if(e.tagName=='INPUT') e.value=v;
                else if(e.tagName=='TEXTAREA') e.value=v.replace(/ /g,"\n");
                else e.innerHTML=v;
            },value);
        }

        if(defki) {
            var sets='';
            var p=defki.split("\n");
            for(var i in p) { if(c(p[i])=='') continue;
                var l=p[i].split(' ');
                if(!pp[l[0]]) {
		    sets+="FILE.key.add /Settings.txt "+l[0]+" "+l[1]+"\n";
		    zabil('settings',"<tr><td align='left'><b>"+h(l[0])+"</b></td><td align='right'>&nbsp;<input size='15' name='"+h(l[0])+"' type='text' value='"+h(l[1])+"'></td></tr>"+vzyal('settings'));
		}
            }
            if(sets!='') AJ(sets,function(s){ salert("Default /Settings.txt saved: "+s,500); });
        }
    }});

    AJ("TIMER.stop"+"\n"+"/UpgradeWebPage",function(s){if(s=="OK") {salert('Page Upgraded',2000); setTimeout('location.reload()',500);}});
}

function ChangeSettings(e) { if(!confirm('Save /Settings.txt?')) return;
    e=e.form;
    var o=''; for(var i=0;i<e.length;i++) { if(e[i].type=='submit') continue; // >
        var val=c(e[i].value); if(val=='') val=e[i].placeholder; val=val.replace(/\n/g,' ');
        o+="FILE.key.add /Settings.txt "+e[i].name+" "+val+"\n";
    }
    AJ(o,function(s){ salert('Saved',1000);});
}

function ESPrestart() { if(!confirm('Restart ESP?')) return;
    AJ("play DRMFSL\nESP.restart");
    document.body.innerHTML="<img src='ajaxm.gif' style='width:10%;height:10%;margin:10%;'>";
    setTimeout('document.location.reload(true)',3000);
}

function getPstyle(nod){ return (typeof(nod)=='object'?nod:idd(nod)).style; }

function aFlash(p,x){ p=getPstyle(p); if(!x)x=0.5;
 // p.transitionProperty='transform'; // p.animation='none'; 
 p.transitionDuration=x+'s'; p.transform='scale(1.7)';
 setTimeout(function(){ p.transitionDuration=2*x+'s'; p.transform='scale(1)';},x*2000);
 setTimeout(function(){ p.transform='unset'; },x*4200);
}

function a360(p,x){ p=getPstyle(p); if(!x)x=1;
 p.transitionDuration=x+'s'; p.transform='rotate(360deg)';
 setTimeout(function(){ p.transform='unset'; },x*1010);
}






// ===== MERA - update

function wait_wifi(){
 setTimeout(function(){ AJAX(WWWAJAX,{ timeout:2000,noajax:1,error:wait_wifi,ontimeout:wait_wifi,callback:function(s){
 if(c(s)=="wait") wait_wifi();
 else { aFlash('wifi_id'); wifi_print(s); }
 } },"echo {WiFi.scanComplete}"); },300);
}

function getwifi(e){
    p=getPstyle('wifi_id'); p.transitionDuration='120s'; p.transform='rotate('+(360*120)+'deg)';
    AJ("WiFi.scanNetworks true\nplay mrd");
    wait_wifi();
}

FW={};
function FWhis(md5){
    var o='<p>';
    for(var i in FW) {
	o+="<div>"+FW[i].t
	    +(i==md5
	    ?" <div style='display:inline-block;color:green'>"
	    : " <div class='ll' onclick='new_firmware(this,\""+FW[i].file+"\")'>"
	    )
	+FW[i].VER+"</div> «"+FW[i].VERNAME+"»</div>";
    }
    zabil('FW',o);
}

function wait_upgrade(){
 setTimeout(function(){
 AJAX(WWWAJAX,{ timeout:2000,noajax:1,error:wait_upgrade,ontimeout:wait_upgrade,callback:
 function(us) { if(1*us!=1) wait_upgrade();
  else {
    aFlash('upgrade_id');

    AJ("echo serv=={server_update}@@@ip=={ip}@@@VER=={VER}@@@VERNAME=={VERNAME}@@@MD5=={getSketchMD5}@@@ESP_TYPE=={ESP_TYPE}",
    function(se){ se=c(se).split('@@@');
    	for(var i in se) { var a=se[i].split('=='); se[a[0]]=a[1]; if(idd(a[0])) zabil(a[0],a[1]); delete(se[i]); }
	if(se['ESP_TYPE']=='') se['ESP_TYPE']="ESP_12";
	var esp=se['ESP_TYPE'].replace(/^ESP_/g,'');
        AJAX('http://lleo.me/ESP8266/index.php?justfile=firmware.new/firmware'+esp+'.txt',{ timeout:2000,noajax:1,callback:
	function(fws) {
	    fws=c(fws); fws=(fws==''?[]:fws.split('\n')); // fws=fws[fws.length-1].split(' ');
	    var last=0;
	    for(var i in fws) {
		var w=fws[i].split(' ');
		var VN=''; for(var j=4;j<w.length;j++) VN+=(VN==''?'':' ')+w[j]; //>
		last=w[3];
		var D = new Date(1000*w[2]); D = D.toLocaleString();
		FW[w[3]]={file:w[0],VER:w[1],t:D,md5:w[3],VERNAME:VN};
	    }
		var o="<tt>Your firmware "+se['ESP_TYPE']+": "+se.VER+' «'+se.VERNAME+"»"+(FW[se.MD5]?" "+FW[se.MD5].t:'');
		if(last==0 || last==c(se.MD5)) o+=" <font color='green'>&mdash; is last</font>";
		else {
		    if(last!==0) o+="<br><font color='red'>Last firmware "+se['ESP_TYPE']+": "+FW[last].VER+" «"+FW[last].VERNAME+"»</font> "+FW[last].t
		    +"<br><input type='button' value='Upgrade' onclick='new_firmware(this,\""+FW[last].file+"\")'>";
		}
		if(last!==0) o+="<div id='FW'><div onclick=\"FWhis('"+se.MD5+"')\" class='l'>[firmware history]</div></div>";
        	zabil('firmware',o+"</tt>");
    	}});
	// end AJAX http://lleo

    });
    // end AJ echo serv==

   } // end else

  }},"echo {UPGRADE}");
  // end AJAX WWWAJAX

  },1000);
  // end timeOut
}

function getsofts(e){
    p=getPstyle('upgrade_id'); p.transitionDuration='120s'; p.transform='rotate('+(360*120)+'deg)';
    AJ("UPGRADE = 0\nupgrade\nUPGRADE = 1\nplay drm");
    wait_upgrade();
}

function new_firmware(e,file) {
    if(e) clean(e);
    if(confirm('Upgrade to '+file+'?'))
        AJ("upgrade.firmware http://lleo.me/ESP8266/firmware.new/"+file,function(s){salert('Working...')});
}

function execute(v) { if(c(v)!='') AJ(v,function(s){ idie(s,"<span style='cursor:pointer' alt='https://lleo.me/ESP8266/cfaq' onclick='go(this)'>System Console</span>") }); }
function go(e) { document.location=e.getAttribute('alt'); }
