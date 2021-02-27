<?php /* Smarty version 3.1.24, created on 2015-11-12 08:13:23
         compiled from "C:/xampp/htdocs/out/pinakure/stuff/include/dispatch.js" */ ?>
<?php
/*%%SmartyHeaderCode:2237556443c13cfcb63_36579487%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '33c662b844b0e1fde625204078aa6866c332ffa6' => 
    array (
      0 => 'C:/xampp/htdocs/out/pinakure/stuff/include/dispatch.js',
      1 => 1447312392,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '2237556443c13cfcb63_36579487',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_56443c14006f84_47532965',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_56443c14006f84_47532965')) {
function content_56443c14006f84_47532965 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '2237556443c13cfcb63_36579487';
?>
function AJAX(request){
	vpu.pause();
	vpu.console.print("^6DISPATCH : ^dSending ^c"+request+"^d request");
		
	$.post( "dispatch.php?width="+parseInt(vpu.width)+"&height="+parseInt(vpu.height), { page: request })
		.done(function( data ) {
		var d;
		try {
			d = JSON.parse(data);
		} catch(err){
			vpu.console.print("^6RESPONSE : ^cReceived ^f'"+data+"'");
			vpu.console.print("^6           ^5Cannot create Object: ^3'"+err.message+"'");
			return;
		}
		
		report("RESPONSE", "Received ^e"+d.command+" ^7statement");
		
		switch(d.command){
			case 'blit':
				var dSize = d.width * d.height;
				var i;
				vpu.clean();
				for(i = 0; i < dSize; i++){
					vpu.mapData0_0[i] = d.data0[i];
					vpu.mapData0_1[i] = d.data1[i];
					vpu.mapData0_2[i] = d.data2[i];
					vpu.map.maps[0][i] = d.data0[i];
					vpu.map.maps[1][i] = d.data1[i]&0x0FFF;
					vpu.map.maps[2][i] = d.data2[i]&0x0FFF;
					vpu.map.pals[1][i] = (d.data1[i]&0xF000 )>>12;
					vpu.map.pals[2][i] = (d.data2[i]&0xF000 )>>12;			
				}
				vpu.resume();		
				vpu.console.dump();
				break;
		
			case 'print':
				vpu.console.print(d.text);
				break;
		
			case 'program':
				vpu.console.print("^bLOADING PROGRAM ^7" + d.name + "^b...");
				try {
					eval(d.data);
				} catch(err){
					vpu.console.print("^5ERROR :^3 "+err.message);
					return;
				}
				vpu.console.print("^fCode Load was sucessful!");
				break;

			case 'container':
				var c = new Container();
				c.deserialize(d.container);
				c.dump();
				
				// Once the container object is created, request its code
				$.post( "dispatch.php", { page: "code", program: c.name }).done(function(data) {
					var d;
					try {						
						d = JSON.parse(data);
						c.setCode(d.code);
						c.init(vpu.system);
						// Finally add it to the active container queue
						vpu.system.addContainer(c);
					} catch(err){
						vpu.console.print("^6CONTAINER CODE ERROR : ^cReceived ^f'"+data+"'");
						vpu.console.print(  "                       ^5Cannot create Object: ^3'"+err.message+"'");
						return;
					}
				})(c, d);
				break;
		}
	 });		
}<?php }
}
?>