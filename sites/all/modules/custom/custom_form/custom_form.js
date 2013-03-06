
function check_value(a)
{
	if(a=='' || a=='Ankit')
	{
		alert('please enter your name');
		$('#custom-id').val('');
		$('#custom-id').focus();
		 return false;
		
		
	}
}

$(document).ready(function() {
	
	
	var aa = $('input[name$="textfield-second-name"]');
	
	aa.click(function(){
		aa.val('');
		
	});

	aa.blur(function(){
		if(aa.val()=='' )
		{
		 aa.val('kumar');
		}
				
	});
	

	var bb = $('input[name$="father-name"]');
	
	bb.click(function(){
		bb.val('');
		
	});

	bb.blur(function(){
		if(bb.val()=='' )
		{
		 bb.val('father\'s name');
		}
				
	});
	
})
