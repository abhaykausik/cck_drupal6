<?php
// $Id$

/**
* This file is created to render priceing form for church business sponsor
*/
  
/**
*  Form creation
*/
function custom_non_church_business_spnosor()
{
	
	$output='';
	$output .= drupal_get_form('custom_non_church_business_sponsor_form');
	return $output;

}

function custom_non_church_business_sponsor_form(){
	$plan = array("monthly", "quarterly", "annual");
    $plan_cnt = count($plan);
	$form = array();
	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			 $exist_value = db_result(db_query("SELECT $plan[$i] FROM {business_ad_price_calc} WHERE tid=%d AND is_church=0",$row->tid));
			 $table_subid_field = 'sid_'.$plan[$i];
			 $exist_sid = db_result(db_query("SELECT $table_subid_field FROM {business_ad_price_calc} WHERE tid=%d AND is_church=0",$row->tid));
			 if($exist_value)
			{
				 $default_value = $exist_value;
			}
			else
			{
				$default_value=0;
			}
			
			if($i==0)
			{
				 $form['cnbsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => t($row->name),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#description' => t('Enter Business Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and subscription is <b>'.$exist_sid.'</b>'),
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' =>  $default_value,
					                    

									   );
			}
			else if($i==($plan_cnt-1))
			{
				 $form['cnbsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
										'#default_value' =>  $default_value,
					                    '#size' => 20,
					                    '#description' => t('Enter Business Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and subscription is <b>'.$exist_sid.'</b>'),
					                    '#attributes' => array('class' => 'c-component-3'),
					                    '#suffix' => '</div>',

									   );
			}
			else
			{
				 $form['cnbsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                     '#description' => t('Enter Business Sponsor <b>'.$plan[$i].' </b>  price charge having no. of memebers <b>'.$row->name.'</b> and subscription is <b>'.$exist_sid.'</b>'),
					                    '#attributes' => array('class' => 'c-component-2'),
										'#default_value' => $default_value,

									   );
			}
		}
	}
	
	
  $form['non-church-business-submit'] = array(
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
function custom_non_church_business_sponsor_form_validate($form, &$form_state){

	$plan = array("monthly", "quarterly", "annual");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cnbsf-'.$row->tid."-".$plan[$i];
           
		   if(!is_numeric($form_state['values'][$form_field_name]) && $form_state['values'][$form_field_name]!='0'){
			form_set_error($form_field_name, t('Please enter numeric value in  '.$plan[$i].' cloumn of field '.$row->name));
		   }
			
			
		}
	}
}

/**
*  Form submit
*/
function custom_non_church_business_sponsor_form_submit($form, &$form_state){

	$plan = array("monthly", "quarterly", "annual");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=11 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cnbsf-'.$row->tid."-".$plan[$i];
			
			$field_value = $form_state['values'][$form_field_name];
			           
		   $exist_value = db_result(db_query("SELECT tid FROM {business_ad_price_calc} WHERE tid=%d AND is_church=0",$row->tid));
		   $table_subid_field = 'sid_'.$plan[$i];
		   
		   if($exist_value)
			{
			   $exist_sid = db_result(db_query("SELECT $table_subid_field FROM {business_ad_price_calc} WHERE tid=%d AND is_church=0",$row->tid));
			   if($exist_sid==0)
			   {
				   $church_businesssponsor_first_arg = $row->name."-".$plan[$i];
				   $reoccuring =1;
			       $subid = non_church_business_sponsor_subscription_creation($church_businesssponsor_first_arg, $field_value, $reoccuring);
				   db_query("UPDATE {business_ad_price_calc} SET $table_subid_field = %d WHERE tid=%d AND is_church=0", $subid, $row->tid);
			   }
			   else
				{
				   non_church_business_sponsor_subscription_update($field_value, $exist_sid);
				}
			   db_query("UPDATE {business_ad_price_calc} SET $plan[$i] = %f WHERE tid=%d AND is_church=0", $field_value, $row->tid);
			}
			else
			{
                    $church_businesssponsor_first_arg = $row->name."-".$plan[$i];
					$reoccuring =1;
			        $subid = non_church_business_sponsor_subscription_creation($church_businesssponsor_first_arg, $field_value, $reoccuring);
					

              db_query("INSERT INTO {business_ad_price_calc} (tid, is_church, $plan[$i], $table_subid_field) VALUES (%d, 0, %f, %d)",$row->tid, $field_value, $subid);
			}
			
		}
	}
	
	drupal_set_message('Data has been updated');

}


function non_church_business_sponsor_subscription_creation($a, $amount, $reoccuring)
{
   $plan_arr = array('monthly'=>1, 'quarterly'=>3, 'annual'=>12);

   $data_arr = explode("-", $a);
   
     $item_name = 'Business sponsor '.$data_arr[1].' plan for Non Church with '.$data_arr[0].' members';
	 $description = 'Business sponsor '.$data_arr[1].' plan for Non Church with '.$data_arr[0].' members description';
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

	  $sql = "INSERT INTO {lm_paypal_subscriptions} (item_name, description, Kind, a3, p3, src, t3, t1, t2, currency_code, status, uid_admin, send_admin_onsub,  send_admin_onend,  send_user_onsub, send_user_onsub_subject, send_user_onsub_body, nearend_days, send_user_onnearend, send_user_onnearend_subject, send_user_onnearend_body, send_user_onend, send_user_onend_subject, send_user_onend_body) VALUES ('%s', '%s', %d, %f, %d, %d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s', %d, '%s', '%s')";

	  $insert_id = db_query($sql, $item_name, $description, $Kind, $a3, $p3, $src, $t3, $t1, $t2, $currency_code, $status, $uid_admin, $send_admin_onsub, $send_admin_onend, $send_user_onsub, $send_user_onsub_subject, $send_user_onsub_body, $nearend_days, $send_user_onnearend, $send_user_onnearend_subject, $send_user_onnearend_body, $send_user_onend, $send_user_onend_subject, $send_user_onend_body);
	 


       
	  
	   if($insert_id)
		 {
		  $table = '{lm_paypal_subscriptions}';
		  $field = 'subid';
		  $last_subid = db_last_insert_id($table, $field);
		  return $last_subid;

		 }
		

 
}

function non_church_business_sponsor_subscription_update($amount, $sid)
{
	db_query("UPDATE {lm_paypal_subscriptions} SET a3 =%f  WHERE subid=%d", $amount, $sid );
}




