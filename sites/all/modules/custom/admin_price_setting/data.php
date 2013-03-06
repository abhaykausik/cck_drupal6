<?php
 
function more_than_one_month_update($term_id, $term_name, $field_name, $field_value)
{
          $personal_plan_sid = array("pmonth"=>1, "quarterly"=>3, "annual"=>12, "2"=>2, "3"=>3, "4"=>4, "5"=>5, "6"=>6, "7"=>7, "8"=>8, "9"=>9, "10"=>10, "11"=>11, "12"=>12);
		  
		  foreach($personal_plan_sid as $key => $value)
		  {
			  if($key=='pmonth')
			  {
				  // only data base insertion no subid creation

				 // db_query("INSERT INTO {personal_ad_price_calc} (tid, is_church, $key) VALUES (%d, 1, %f)",$term_id, $field_value);
				  
			  }
			  else if($key=='quarterly')
			  {
				  // both database insertion as well as subid creation

				  
				   $amount = 3*$field_value;
				   $sid_quarterly = db_result(db_query("SELECT sid_quarterly FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d", $term_id, 1));

				   church_persoanl_sponsor_subscription_update($amount, $sid_quarterly);

				   db_query("UPDATE {personal_ad_price_calc} SET $key=%f WHERE tid=%d AND is_church=%d", $subid_quater, $amount, $term_id, 1);

				   
			  }
			  else if($key=='annual')
			  {
				  $amount = 12*$field_value;
				 $sid_annual = db_result(db_query("SELECT sid_annual FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d", $term_id, 1));

				   church_persoanl_sponsor_subscription_update($amount, $sid_annual);
				   
				  db_query("UPDATE  {personal_ad_price_calc} SET $key=%f WHERE tid=%d AND is_church=%d", $subid_annual, $amount, $term_id, 1);
				   
			  }
              
			  $sid_pmonth = db_result(db_query("SELECT sid_pmonth FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d", $term_id, 1));
			  $sid_pmonth_arr = explode(",", $sid_pmonth);

				   
			   else if($key=='2')
			  {
				   // no data base insertion only subid generation

				   
				   $amount = $personal_plan_sid[$key]*$field_value;
				   church_persoanl_sponsor_subscription_update($amount, $sid);
				   
			  }
			  else
			  {
				  // no data base insertion only subid generation

				  
				  $amount = $personal_plan_sid[$key]*$field_value;
				  church_persoanl_sponsor_subscription_update($amount, $sid);
			  }
			  


		  }

		  
		  

}

?>
