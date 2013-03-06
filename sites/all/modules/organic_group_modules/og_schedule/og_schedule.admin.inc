<?php

/**
 * Page callback function for admin/og/schedule.
 *
 * @return string
 */
function og_schedule_admin_page() {
  $fields = _content_type_info();
  #$fields = $fields['fields'];
  $fields = $fields['content types'];
  //var_export($fields);
  return drupal_get_form('og_schedule_admin_page_form');
}

/**
 * Implementation of hook_form().
 *
 * @return array
 */
function og_schedule_admin_page_form() {
  $form = array(
    'og_schedule_track_type' => array(
      '#type' => 'select',
      '#title' => t('Track content type'),
      '#default_value' => variable_get('og_schedule_track_type', NULL),
      '#options' => _og_schedule_get_content_types(),
      '#description' => t(''),
    ),
    'og_schedule_session_type' => array(
      '#type' => 'select',
      '#title' => t('Session content type'),
      '#default_value' => variable_get('og_schedule_session_type', NULL),
      '#options' => _og_schedule_get_content_types(),
      '#description' => t('This must have the floowing two CCK'),
    ),
    'og_schedule_session_date_cck' => array(
      '#type' => 'select',
      '#title' => t('Session date CCK field'),
      '#default_value' => variable_get('og_schedule_session_date_cck', NULL),
      '#description' => t('This field must be a <em>date</em> field, which is <em>required</em>, and has a <em>required todate</em>.'),
      '#options' => _og_schedule_get_date_fields(),
    ),
    'og_schedule_session_nodereference_cck' => array(
      '#type' => 'select',
      '#title' => t('Session nodereference CCK field'),
      '#default_value' => variable_get('og_schedule_session_nodereference_cck', NULL),
      '#description' => t('This field must be a <em>nodereference</em>, which is <em>required</em>. This must point to the track content type.'),
      '#options' => _og_schedule_get_reference_fields(),
    ),
  );
  $sysform = system_settings_form($form);
  $sysform['#validate'] = array('og_schedule_admin_page_form_validate');
  return $sysform;
}

/**
 * Implementation of hook_form_validate().
 *
 * @param array $form
 * @param array $form_state
 */
function og_schedule_admin_page_form_validate($form, $form_state) {
  $info = _content_type_info();
  $content_types = $info['content types'];
  $values = $form_state['values'];
  if(!isset($content_types[$values['og_schedule_session_type']]['fields'][$values['og_schedule_session_date_cck']]))
    form_set_error('og_schedule_session_date_cck',
      t('The %ct does not have %field',
        array('%ct'=>$values['og_schedule_session_type'], '%field'=>$values['og_schedule_session_date_cck'])));
  if(!isset($content_types[$values['og_schedule_session_type']]['fields'][$values['og_schedule_session_nodereference_cck']]))
    form_set_error('og_schedule_session_nodereference_cck',
      t('The %ct does not have %field',
        array('%ct'=>$values['og_schedule_session_type'], '%field'=>$values['og_schedule_session_nodereference_cck'])));
  else if($content_types[$values['og_schedule_session_type']]['fields']
          [$values['og_schedule_session_nodereference_cck']]['referenceable_types'][$values['og_schedule_track_type']] === 0)
    form_set_error('og_schedule_session_nodereference_cck',
      t('The %field does not refences %ct',
        array('%field'=>$values['og_schedule_session_nodereference'], '%ct'=>$values['og_schedule_track_type'])));
}

/**
 * Returns group post and group wiki content types.
 *
 * @return array
 */
function _og_schedule_get_content_types() {
  $types = array();
  foreach(node_get_types() as $t) {
    if(og_is_group_post_type($t->type) || og_is_wiki_type($t->type)) {
      $types [$t->type]= $t->name;
    }
  }
  return $types;
}

/**
 * Returns date type fields.
 *
 * @staticvar array $cache
 * @return array
 */
function _og_schedule_get_date_fields() {
  static $cache = NULL;
  if($cache === NULL) {
    $info = _content_type_info();
    $cache = array();
    foreach($info['fields'] as $k => $v) {
      if($v['type'] != 'date') continue;
      if(!$v['required']) continue;
      if(!$v['active']) continue;
      if(!isset($v['todate']) || $v['todate'] != 'required') continue;
      $cache[$k] = $k;
    }
  }
  return $cache;
}

/**
 * Returns nodereference type fields.
 *
 * @staticvar array $cache
 * @return array
 */
function _og_schedule_get_reference_fields() {
  static $cache = NULL;
  if($cache === NULL) {
    $info = _content_type_info();
    $cache = array();
    foreach($info['fields'] as $k => $v) {
      if($v['type'] != 'nodereference') continue;
      if(!$v['required']) continue;
      if(!$v['active']) continue;
      $cache[$k] = $k;
    }
  }
  return $cache;
}