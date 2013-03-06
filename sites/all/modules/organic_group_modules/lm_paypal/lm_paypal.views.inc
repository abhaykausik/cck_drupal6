<?php
/**
 * @file
 */

/**
 * Implementation of hook_views_data().
 */
function lm_paypal_views_data() {
  // Instant Payment Notifications table.
  $lm_paypal_boolean_fields = array('processed', 'test_ipn', 'recurring', 'reattempt');
  $lm_paypal_datetime_fields = array('timestamp', 'payment_timestamp');
    
  $data = array();
  $data['lm_paypal_ipns']['table']['group']  = t('LM PayPal');
  
  $data['lm_paypal_ipns']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Instant Payment Notification'),
    'help' => t('Table of IPNs received from PayPal.'),
    'weight' => 20,
  );

  // Re-use the field names and descriptions from the schema - we do not want to re-type all 70+ fields.
  $schema = drupal_get_schema('lm_paypal_ipns');

  $numeric_defaults = array(
    'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $boolean_defaults = array(
    'field' => array('handler' => 'views_handler_field_boolean', 'click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $text_defaults = array(
    'field' => array('click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $date_defaults = array(
    'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_date'),
    'argument' => array('handler' => 'views_handler_argument_date'),
    'sort' => array('handler' => 'views_handler_sort_date'),
  );
  
  $types = array();
  foreach ($schema['fields'] as $fieldname => $info) {

    $types[$info['type']][] = $fieldname; // Used during development to check that all types have been catered for.
    switch ($info['type'])  {
      case 'varchar':
      case 'text':
        $defaults = $text_defaults;
        break;
      case 'numeric':
        $defaults = $numeric_defaults;
        if ($info['scale'] > 0) {
          $defaults['field']['float'] = TRUE;
          $defaults['field']['set_precision'] = TRUE;
          $defaults['field']['precision'] = $info['scale'];
        }
        break;
      case 'int':
        if (in_array($fieldname, $lm_paypal_boolean_fields)) {
          $defaults = $boolean_defaults;
        }
        else if (in_array($fieldname, $lm_paypal_datetime_fields)) {
          $defaults = $date_defaults;
        }
        else {
          $defaults = $numeric_defaults;
        }
        break;
      case 'serial':
        $defaults = $numeric_defaults;
        break;
    }
    
    $data['lm_paypal_ipns'][$fieldname] = array_merge($defaults, array(
       // Make the title more readable, less like a db column name.
      'title' => ucfirst(str_replace('_', ' ', $fieldname)),
       // Capitalise first letter of description, but do not use ucfirst so as to preserve existing capitalisation.
      'help' => strtoupper(substr($info['description'], 0, 1)) . substr($info['description'],1),
    ));
  }
  
  return $data;
}

