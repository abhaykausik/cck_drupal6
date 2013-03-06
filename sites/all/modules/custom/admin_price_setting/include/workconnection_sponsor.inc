<?php
// $Id$

/**
* This file is created to show all links of admin price setting
*/

function custom_workconnection_spnosor()
{
	$output='';
	$output .= drupal_get_form('custom_workconnection_sponsor_form');
	return $output;
}


function custom_workconnection_sponsor_form()         
	{
		$existing_sid = variable_get('workconnection-sid-new', '');
	
	$form = array();
	$form['workconnection-fixed-price']= array(
										'#title' => t('Work connection Price'),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#suffix' => '</div>',
		                                '#description' => t('Enter Work connetion fixed price having subid is <b>'.$existing_sid.'</b>'),
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' => (variable_get('workconnection-fixed-price','')) ? (variable_get('workconnection-fixed-price','')):0,
					                    

									   );
	$form['business-sponsor-minp']= array(
										'#title' => t('Business Sponsor Min Price'),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#suffix' => '</div>',
		                                '#description' => t('Enter Business Sponsor Minium price'),
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' => (variable_get('business-sponsor-minp','')) ? (variable_get('business-sponsor-minp','')):0,
					                    
					                    

									   );
 $form['personal-sponsor-minp']= array(
										'#title' => t('Personal sponsor Min Price'),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#suffix' => '</div>',
	                                    '#description' => t('Enter Personal Sponsor Minium price'),
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' => (variable_get('personal-sponsor-minp','')) ? (variable_get('personal-sponsor-minp','')):0,
					                    
					                    

									   );
	$form['church-workconnection-submit'] = array(
	                                        '#type' => 'submit',
	                                        '#prefix' => '<div class="c-submit">',
	                                        '#suffix' => '</div>',
	                                        '#value'=> t('Save'),
	                                       );
										   
	
	
	
  
  return $form;
  //return system_settings_form($form);
}



/**
*  Form validation
*/

function custom_workconnection_sponsor_form_validate($form, &$form_state){

	if(!is_numeric($form_state['values']['workconnection-fixed-price']) && $form_state['values']['workconnection-fixed-price']!='0'){
			form_set_error('workconnection-fixed-price', t('Please enter numeric value in Work connection Price '));
	}

	if(!is_numeric($form_state['values']['business-sponsor-minp']) && $form_state['values']['business-sponsor-minp']!='0'){
			form_set_error('business-sponsor-minp', t('Please enter numeric value in Personal sponsor Min Price'));
	}

	if(!is_numeric($form_state['values']['personal-sponsor-minp']) && $form_state['values']['personal-sponsor-minp']!='0'){
			form_set_error('personal-sponsor-minp', t('Please enter numeric value in Business Sponsor Min Price '));
	}

	
	
}

function custom_workconnection_sponsor_form_submit($form, &$form_state){

	$workconnection = $form_state['values']['workconnection-fixed-price'];
	$business_sponsor_min = $form_state['values']['business-sponsor-minp'];
	$personal_sponsor_min = $form_state['values']['personal-sponsor-minp'];

    
	variable_set('business-sponsor-minp', $business_sponsor_min);
	variable_set('personal-sponsor-minp', $personal_sponsor_min);
	variable_set('workconnection-fixed-price', $workconnection);
	if(variable_get('workconnection-sid-new', ''))
	{
		$existing_sid = variable_get('workconnection-sid-new', '');
		workconnection_subscription_update($workconnection, $existing_sid);
		
	}
	else
	{
		$workconnection_first_arg = $row->name."-annual";
		$reoccuring =1;
		$workconnection_sid = workconnection_subscription_creation($workconnection_first_arg, $workconnection, $reoccuring);
		variable_set('workconnection-sid-new', $workconnection_sid);
		
	}
	

	drupal_set_message('Data has been saved');
}

function workconnection_subscription_creation($a, $amount, $reoccuring)
{
   $plan_arr = array('monthly'=>1, 'quarterly'=>3, 'annual'=>12);

   $data_arr = explode("-", $a);
   
     $item_name = 'Work Connection '.$data_arr[1].' plan  ';
	 $description = 'Work Connection '.$data_arr[1].' plan  description';
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

function workconnection_subscription_update($amount, $sid)
{
	db_query("UPDATE {lm_paypal_subscriptions} SET a3 =%f  WHERE subid=%d", $amount, $sid );
}









