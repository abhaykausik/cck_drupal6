<?php
// $Id$

/**
* This file is created to render priceing form for church business sponsor
*/
  
/**
*  Form creation
*/
function custom_church_personal_spnosor()
{
	
	$output='';
	$output .= drupal_get_form('custom_church_personal_sponsor_form');
	return $output;

}

function custom_church_personal_sponsor_form()         
	{
	// pmonth means monthly charge for more than one month
	$plan = array("pmonth", "monthly");
	
    $plan_cnt = count($plan);
	$form = array();
	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			 $exist_value = db_result(db_query("SELECT $plan[$i] FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=1",$row->tid));
			 if($exist_value)
			{
				 $default_value = $exist_value;
			}
			else
			{
				$default_value=0;
			}

			if($plan[$i]=='pmonth')
			{
				$description1 = db_result(db_query("SELECT sid_quarterly FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d",$row->tid, 1));
				$description2 = db_result(db_query("SELECT sid_annual FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d",$row->tid, 1));
				$description3 = db_result(db_query("SELECT sid_pmonth FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d",$row->tid, 1));
				$description = $description1.",".$description2.",".$description3;
			}
			if($plan[$i]=='monthly')
			{
				$description = db_result(db_query("SELECT sid_monthly FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d",$row->tid, 1));
			}
			
			if($i==0)
			{
				 $form['cpsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => t($row->name),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#description' => t('Enter Personal Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and Subscription Id <b>'.$description.'</b>'),
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' =>  $default_value,
					                    

									   );
			}
			else if($i==($plan_cnt-1))
			{
				 $form['cpsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
										'#default_value' =>  $default_value,
					                    '#size' => 20,
					                    '#description' => t('Enter Personal Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and Subscription Id <b>'.$description.'</b>'),
					                    '#attributes' => array('class' => 'c-component-3'),
					                    '#suffix' => '</div>',

									   );
			}
			else
			{
				 $form['cpsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#description' => t('Enter Personal Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and Subscription Id <b>'.$description.'</b>'),
					                    '#attributes' => array('class' => 'c-component-2'),
										'#default_value' => $default_value,

									   );
			}
		}
	}
	
	
  $form['church-personal-submit'] = array(
	                                        '#type' => 'submit',
	                                        '#prefix' => '<div class="c-submit">',
	                                        '#suffix' => '</div>',
	                                        '#value'=> t('Save'),
	                                       );
  return $form;
}



/**
*  Form validation
*/

function custom_church_personal_sponsor_form_validate($form, &$form_state){

	$plan = array("pmonth", "monthly");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cpsf-'.$row->tid."-".$plan[$i];
           
		   if(!is_numeric($form_state['values'][$form_field_name]) && $form_state['values'][$form_field_name]!='0'){
			form_set_error($form_field_name, t('Please enter numeric value in  '.$plan[$i].' cloumn of field '.$row->name));
		   }
			
			
		}
	}
}

/**
*  Form submit
*/

function custom_church_personal_sponsor_form_submit($form, &$form_state){

	$plan = array("pmonth", "monthly");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cpsf-'.$row->tid."-".$plan[$i];
			
			$field_value = $form_state['values'][$form_field_name];
			           
		   $exist_value = db_result(db_query("SELECT tid FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=1",$row->tid));
		   
		   if($exist_value)
			{
			   if($plan[$i]=='pmonth')
				{
				   //db_query("UPDATE {personal_ad_price_calc} SET $plan[$i] = %f, quarterly=%f, annual=%f  WHERE tid=%d AND is_church=1", $field_value, 3*$field_value, 12*$field_value,$row->tid);

				    more_than_one_month_update($row->tid, $row->name, $plan[$i], $field_value);
				   
				}
				else
				{
					//db_query("UPDATE {personal_ad_price_calc} SET $plan[$i] = %f WHERE tid=%d AND is_church=1", $field_value, $row->tid);
					one_month_update($row->tid, $row->name, $plan[$i], $field_value);
					
				}
			   
			}
			else
			{

			 if($plan[$i]=='pmonth')
				{
				   
				   //db_query("INSERT INTO {personal_ad_price_calc} (tid, is_church, $plan[$i], quarterly, annual) VALUES (%d, 1, %f, %f, %f)",$row->tid, $field_value, 3*$field_value, 12*$field_value);
				   more_than_one_month_insert($row->tid, $row->name, $plan[$i], $field_value);
				}
				else
				{
					//db_query("INSERT INTO {personal_ad_price_calc} (tid, is_church, $plan[$i]) VALUES (%d, 1, %f)",$row->tid, $field_value);
					one_month_insert($row->tid, $row->name, $plan[$i], $field_value);
				}
              
			}
			
		}
	}
	
	drupal_set_message('Data has been updated');

}


function church_persoanl_sponsor_subscription_creation($a, $amount, $reoccuring)
{
   $plan_arr = array("onemonth_reoccur"=>1, "onetime"=>1, "pmonth"=>1, "quarterly"=>3, "annual"=>12, "2"=>2, "3"=>3, "4"=>4, "5"=>5, "6"=>6, "7"=>7, "8"=>8, "9"=>9, "10"=>10, "11"=>11, "12"=>12);

   $data_arr = explode("-", $a);
   

	 $item_name = 'Persoanl sponsor '.$data_arr[1].' plan for Church with '.$data_arr[0].' members';
	 $description = 'Persoanl sponsor '.$data_arr[1].' plan for Church with '.$data_arr[0].' members description';
	 $a3 = $amount; // subscription amount;
	 $p3 = $plan_arr[$data_arr[1]];  //number of month of day for billing cycle
	 $src = $reoccuring; // for recurring 1 and non-recurring `null`
	 $srt = ''; // no of times reoccuring occurs if 'null' full time.
	 
	 /** These value doesn't change **/
	 $Kind = 1;
	 $t3 = 'M';
	 $t1 = 'D';
	 $t2 = 'D';
	 $currency_code = 'USD';
	 $status = 1;
	 $uid_admin = 1;
	 $send_admin_onsub = 1;
	 $send_admin_onend = 1;
	 $send_user_onsub = 1;
	 $send_user_onsub_subject = 'Welcome to %Subscription%Node';
	 $send_user_onsub_body = 'Hello %Username and welcome to your new subscription %Subscription%Node on %Site';
	 $nearend_days = 5; // no of days befor reminder is send.
	 $send_user_onnearend = 1;
	 $send_user_onnearend_subject = 'Your subscription %Subscription ends soon';
	 $send_user_onnearend_body = 'Hello %Username your subscription to %Subscription on %Site ends in %Days days.';
	 $send_user_onend = 1;
	 $send_user_onend_subject = 'Goodbye from %Subscription';
	 $send_user_onend_body = 'Hello %Username and thank you for being a subscriber to %Subscription on %Site. We hope you will join us again.';

	 if($src==0)
	{
	  $sql = "INSERT INTO {lm_paypal_subscriptions} (item_name, description, Kind, a3, p3, t3, t1, t2, currency_code, status, uid_admin, send_admin_onsub,  send_admin_onend,  send_user_onsub, send_user_onsub_subject, send_user_onsub_body, nearend_days, send_user_onnearend, send_user_onnearend_subject, send_user_onnearend_body, send_user_onend, send_user_onend_subject, send_user_onend_body) VALUES ('%s', '%s', %d, %f, %d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s', %d, '%s', '%s')";

	  $insert_id = db_query($sql, $item_name, $description, $Kind, $a3, $p3, $t3, $t1, $t2, $currency_code, $status, $uid_admin, $send_admin_onsub, $send_admin_onend, $send_user_onsub, $send_user_onsub_subject, $send_user_onsub_body, $nearend_days, $send_user_onnearend, $send_user_onnearend_subject, $send_user_onnearend_body, $send_user_onend, $send_user_onend_subject, $send_user_onend_body);
	}
	else
	{
		$sql = "INSERT INTO {lm_paypal_subscriptions} (item_name, description, Kind, a3, p3, src, t3, t1, t2, currency_code, status, uid_admin, send_admin_onsub,  send_admin_onend,  send_user_onsub, send_user_onsub_subject, send_user_onsub_body, nearend_days, send_user_onnearend, send_user_onnearend_subject, send_user_onnearend_body, send_user_onend, send_user_onend_subject, send_user_onend_body) VALUES ('%s', '%s', %d, %f, %d, %d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s', %d, '%s', '%s')";

	  $insert_id = db_query($sql, $item_name, $description, $Kind, $a3, $p3, $src, $t3, $t1, $t2, $currency_code, $status, $uid_admin, $send_admin_onsub, $send_admin_onend, $send_user_onsub, $send_user_onsub_subject, $send_user_onsub_body, $nearend_days, $send_user_onnearend, $send_user_onnearend_subject, $send_user_onnearend_body, $send_user_onend, $send_user_onend_subject, $send_user_onend_body);
	}
	 


       
	  
	   if($insert_id)
		 {
		  $table = '{lm_paypal_subscriptions}';
		  $field = 'subid';
		  $last_subid = db_last_insert_id($table, $field);
		  return $last_subid;

		 }
		

 
}

function church_persoanl_sponsor_subscription_update($amount, $sid)
{
	db_query("UPDATE {lm_paypal_subscriptions} SET a3 =%f  WHERE subid=%d", $amount, $sid );
}


function one_month_insert($term_id, $term_name, $field_name, $field_value)
{

          $personal_plan_sid = array("onemonth_reoccur"=>1, "onetime"=>1);
		  $per_month_subid='';
		  
		  foreach($personal_plan_sid as $key => $value)
		  {
			  
              
			   if($key=='onemonth_reoccur')
			  {
				   // no data base insertion only subid generation

				   $church_personalsponsor_first_arg = $term_name."-".$key;
				   $amount = $field_value;
				   $reoccuring =1;
				   $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);
				   $per_month_subid .= $subid;
			  }
			  else
			  {
				  // no data base insertion only subid generation

				  $church_personalsponsor_first_arg = $term_name."-".$key;
				  $amount = $field_value;
				  $reoccuring =0;
				  $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);

				  $per_month_subid .= ",". $subid;
			  }
			  


		  }

		  db_query("INSERT INTO {personal_ad_price_calc} (tid, is_church, $field_name, sid_monthly) VALUES (%d, 1, %f, '%s')",$term_id, $field_value, $per_month_subid);
}

function one_month_update($term_id, $term_name, $field_name, $field_value)
{

          $personal_plan_sid = array("onemonth_reoccur"=>1, "onetime"=>1);

		  $subid_field = 'sid_'.$field_name;
          $subid_exist = db_result(db_query("SELECT $subid_field FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d", $term_id, 1)); 
		  
		  foreach($personal_plan_sid as $key => $value)
		  {
			 
			 if($subid_exist==0)
			  {
				  if($key=='onemonth_reoccur')
				  {
					  $church_personalsponsor_first_arg = $term_name."-".$key;
					  $amount = $field_value;
					  $reoccuring =1;
					  $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);
					   $per_month_subid .= $subid;
				  }
				  else
				  {
					  // no data base insertion only subid generation

					  $church_personalsponsor_first_arg = $term_name."-".$key;
					  $amount = $field_value;
					  $reoccuring =0;
					  $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);
					  $per_month_subid .= ",". $subid;
				  }

				  
			  }
			  else
			  {
                   $subid_exist_arr = explode(",", $subid_exist);
				   if($key=='onetime')
				  {
					 $update_subid = $subid_exist_arr['0'];
					 					   
                     church_persoanl_sponsor_subscription_update($field_value, $update_subid);
					   
					   
				  }
				  else
				  {
					  $update_subid = $subid_exist_arr['1'];
					  church_persoanl_sponsor_subscription_update($field_value, $update_subid);
					  
				  }
			  
			  }
			  


		  }

		  if($subid_exist==0)
			{
			  db_query("UPDATE {personal_ad_price_calc} SET sid_monthly='%s', $field_name=%f WHERE tid=%d AND is_church=%d", $per_month_subid, $field_value, $term_id, 1 );
			}
			else
			{
				db_query("UPDATE {personal_ad_price_calc} SET sid_monthly='%s', $field_name=%f WHERE tid=%d AND is_church=%d", $subid_exist, $field_value, $term_id, 1 );
			}

		  
}

function more_than_one_month_insert($term_id, $term_name, $field_name, $field_value)
{
          $personal_plan_sid = array("pmonth"=>1, "quarterly"=>3, "annual"=>12, 2=>2, 3=>3, 4=>4, 5=>5, 6=>6, 7=>7, 8=>8, 9=>9, 10=>10, 11=>11, 12=>12);
		  
		  foreach($personal_plan_sid as $key => $value)
		  {
			  if($key=='pmonth')
			  {
				  // only data base insertion no subid creation

				  db_query("INSERT INTO {personal_ad_price_calc} (tid, is_church, $key) VALUES (%d, 1, %f)",$term_id, $field_value);
			  }
			  else if($key=='quarterly')
			  {
				  // both database insertion as well as subid creation

				  $church_personalsponsor_first_arg = $term_name."-".$key;
				  $amount = 3*$field_value;
				   $reoccuring =1;
				   $subid_quater = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);
				   db_query("UPDATE {personal_ad_price_calc} SET sid_quarterly=%d, $key=%f WHERE tid=%d AND is_church=%d", $subid_quater, $amount, $term_id, 1);
			  }
			  else if($key=='annual')
			  {
				  // both database insertion as well as subid creation
				  $church_personalsponsor_first_arg = $term_name."-".$key;
				  $amount = 12*$field_value;
				   $reoccuring =1;

				   $subid_annual = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);

				   
				  db_query("UPDATE  {personal_ad_price_calc} SET  sid_annual=%d, $key=%f WHERE tid=%d AND is_church=%d", $subid_annual, $amount, $term_id, 1);
				   
			  }
              
			   else if($key==2)
			  {
				   // no data base insertion only subid generation

				   $church_personalsponsor_first_arg = $term_name."-".$key." months (one time payment)";
				   $amount = $personal_plan_sid[$key]*$field_value;
				   $reoccuring =0;
				   $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);
				   $per_month_subid .= $subid;
			  }
			  else
			  {
				  // no data base insertion only subid generation

				  $church_personalsponsor_first_arg = $term_name."-".$personal_plan_sid[$key]." months (one time payment)";
				  $amount = $personal_plan_sid[$key]*$field_value;
				  $reoccuring =0;
				  $subid = church_persoanl_sponsor_subscription_creation($church_personalsponsor_first_arg, $amount, $reoccuring);

				  $per_month_subid .= ",". $subid;
			  }
			  


		  }

		  
		  db_query("UPDATE  {personal_ad_price_calc} SET sid_pmonth='%s' WHERE tid=%d AND is_church=%d", $per_month_subid, $term_id, 1);

}


function more_than_one_month_update($term_id, $term_name, $field_name, $field_value)
{
          $personal_plan_sid = array("pmonth"=>1, "quarterly"=>3, "annual"=>12, 2=>2, 3=>3, 4=>4, 5=>5, 6=>6, 7=>7, 8=>8, 9=>9, 10=>10, 11=>11, 12=>12);
		  $sid_pmonth = db_result(db_query("SELECT sid_pmonth FROM {personal_ad_price_calc} WHERE tid=%d AND is_church=%d", $term_id, 1));
		  $sid_pmonth_arr = explode(",", $sid_pmonth);
		  
		  foreach($personal_plan_sid as $key => $value)
		  {
			  if($key=='pmonth')
			  {
				  // only data base insertion no subid creation

				  db_query("UPDATE  {personal_ad_price_calc} SET pmonth=%f WHERE tid=%d AND is_church=%d", $field_value, $term_id, 1);
				  
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
              
			  	   
			  
		 
		  else
			  {
				  // no data base insertion only subid generation

				  $pm_sid = $sid_pmonth_arr[$personal_plan_sid[$key]-2];
				  $amount = $personal_plan_sid[$key]*$field_value;
				  church_persoanl_sponsor_subscription_update($amount, $pm_sid);
			  }
			  


		  }

		  
		  

}
