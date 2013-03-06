<?php
// $Id$

/**
* This file is created to show all links of admin price setting
*/

function custom_group_spnosor()
{
	$output='';
	$output .= drupal_get_form('custom_group_sponsor_form');
	return $output;
}


function custom_group_sponsor_form()         
	{
	$plan = array("annual");
    $plan_cnt = count($plan);
	$form = array();
	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=12 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			 $exist_value = db_result(db_query("SELECT $plan[$i] FROM {group_ad_price_clac} WHERE tid=%d",$row->tid));
			 $sub_id = db_result(db_query("SELECT sid FROM {group_ad_price_clac} WHERE tid=%d AND $plan[$i]=%f",$row->tid, $exist_value));
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

				 $form['cgsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => t($row->name),
					                    '#prefix' => '<div class="c-one-group">',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#description' => t('Enter Group Sponsor <b> '.$plan[$i].' </b> price charge having no. of memebers <b>'.$row->name.'</b> and subscriptio id is <b>'.$sub_id.'</b>'),
					                    '#suffix' => '</div>',
					                    '#attributes' => array('class' => 'c-component-1'),
										'#default_value' =>  $default_value,
					                    

									   );
			}
			else if($i==($plan_cnt-1))
			{
				 $form['cgsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
										'#default_value' =>  $default_value,
					                    '#prefix' => '<div class="c-one-group">',
					                    '#size' => 20,
					                    '#description' => t('Enter Group Sponsor <b> '.$plan[$i].' </b> price charge having no. of memebers <b>'.$row->name.'</b> and subscriptio id is <b>'.$sub_id.'</b>'),
					                    '#attributes' => array('class' => 'c-component-3'),
					                    '#suffix' => '</div>',

									   );
			}
			else
			{
				 $form['cgsf-'.$row->tid."-".$plan[$i]]= array(
										'#title' => '',
										'#type'  => 'textfield',
					                    '#size' => 20,
					                    '#prefix' => '<div class="c-one-group">',
					                    '#description' => t('Enter Group Sponsor <b> '.$plan[$i].' </b> price charge having no. of memebers <b>'.$row->name.'</b> and subscriptio id is <b>'.$sub_id.'</b>'),
					                    '#attributes' => array('class' => 'c-component-2'),
										'#default_value' => $default_value,
					                    '#suffix' => '</div>',

									   );
			}
		}
	}
	
	
  $form['church-group-submit'] = array(
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

function custom_group_sponsor_form_validate($form, &$form_state){

	$plan = array("annual");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=12 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cgsf-'.$row->tid."-".$plan[$i];
           
		   if(!is_numeric($form_state['values'][$form_field_name]) && $form_state['values'][$form_field_name]!='0'){
			form_set_error($form_field_name, t('Please enter numeric value in  '.$plan[$i].' cloumn of field '.$row->name));
		   }
			
			
		}
	}
}

/**
*  Form submit
*/

function custom_group_sponsor_form_submit($form, &$form_state){

	$plan = array("annual");
    $plan_cnt = count($plan);

	$sql = "SELECT td.tid, td.name FROM {term_data} AS td  WHERE td.vid=12 ORDER BY td.weight ASC";
	$res = db_query($sql);
	 while($row = db_fetch_object($res))
	{
		 $i==0;
		 for($i=0; $i< $plan_cnt; $i++)
		{
			$form_field_name = 'cgsf-'.$row->tid."-".$plan[$i];
			
			
			$field_value = $form_state['values'][$form_field_name];
			           
		   $exist_value = db_result(db_query("SELECT tid FROM {group_ad_price_clac} WHERE tid=%d",$row->tid));
		   
		   if($exist_value)
			{
			        $subid = db_result(db_query("SELECT sid FROM {group_ad_price_clac} WHERE tid=%d",$row->tid));
					if($subid == 0)
					{
						$groupsponsor_first_arg = $row->name."-".$plan[$i];
					    $reoccuring =1;
			            $subid = group_subscription_creation($groupsponsor_first_arg, $field_value, $reoccuring);

						db_query("UPDATE {group_ad_price_clac} SET sid = %d WHERE tid=%d", $subid, $row->tid);
						
					}
					else
				    {
						group_subscription_update($field_value, $subid);
						
					}
				    
					db_query("UPDATE {group_ad_price_clac} SET $plan[$i] = %f WHERE tid=%d", $field_value, $row->tid);
					
				
			   
			}
			else
			{
				    
                    $groupsponsor_first_arg = $row->name."-".$plan[$i];
					$reoccuring =1;
			        $subid = group_subscription_creation($groupsponsor_first_arg, $field_value, $reoccuring);

					db_query("INSERT INTO {group_ad_price_clac} (tid, $plan[$i], sid) VALUES (%d, %f, %d)",$row->tid, $field_value, $subid);
				
              
			}
			
		}
	}
	
	drupal_set_message('Data has been updated');

}


function group_subscription_creation($a, $amount, $reoccuring)
{
   $plan_arr = array('monthly'=>1, 'quarterly'=>3, 'annual'=>12);

   $data_arr = explode("-", $a);
   

	 $item_name = 'Group Sponsor '.$data_arr[1].' plan  with '.$data_arr[0].' members';
	 $description = 'Group Sponsor '.$data_arr[1].' plan with '.$data_arr[0].' members description';
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

function group_subscription_update($amount, $sid)
{
	db_query("UPDATE {lm_paypal_subscriptions} SET a3 =%f  WHERE subid=%d", $amount, $sid );
}


