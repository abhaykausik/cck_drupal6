<?php
      
	  drupal_add_css( drupal_get_path('module', 'admin_price_setting').'/css/church-business-sponsor.css','module' ,'all' , false );
?>
<h2>Non Church Business Sponsor</h2>
<div class="c-header">

     <label for="edit-cbsf-132-monthly">Number of memebers</label>

	  <div  class="upper">
	  <div class="form-text c-component-1" >Monthly</div>
	</div>
	
	<div  class="upper">
	 <div class="form-text c-component-2" >Quarterly</div>
	</div>
	<div  class="upper">
	 <div class="form-text c-component-3" >Annual</div>
	</div>

</div>
    
<?php	
	print drupal_render($form);


?>

<script type="text/javascript">
$(document).ready(function(){
	$('.c-one-group:even').addClass('c-even');
	$('.c-one-group:odd').addClass('c-odd');
});
</script>