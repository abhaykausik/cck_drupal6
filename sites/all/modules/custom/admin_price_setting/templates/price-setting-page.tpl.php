<?php
    drupal_add_css( drupal_get_path('module', 'admin_price_setting').'/css/church-business-sponsor.css','module' ,'all' , false );
    
?>

<h2>Sponsor Price Setting</h2>
<div id="price-setting-menu-item">
     <ul class="prs">
	    <?php
		      foreach($price_set_menu as $key => $value)
			  {
                echo "<li>".l($value, $key)."</li>";
			  }
		?>
	     
		 
	 </ul>
</div>