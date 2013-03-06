<?php
// $Id: og_global_limits.admin.inc,v 1.4.2.1 2009/07/15 02:54:05 elliottf Exp $

/**
 * This file handles the administrative
 *  functions for the og_global_limits module.
 *
 * @file: og_global_limits.admin.inc
 * @author: Elliott Foster
 * @copyright: NewMBC 2009
 */

/**
 * og_global_limits_settings - return the system settings
 *  form for the og_global_limits module.
 */
function og_global_limits_settings() {
  $form = array();

  $types = variable_get('og_global_limits_group_types', array());

  foreach ($types as $type) {
    $form['og_global_limts_' . $type] = array(
      '#type' => 'fieldset',
      '#title' => t('@type Limits', array('@type' => $type)),
      '#description' => t('Set global limits for the @type group node type.', array('@type' => $type)),
      '#collapsable' => TRUE,
      '#collapsed' => FALSE
    );
    $form['og_global_limts_' . $type]['og_global_limits_' . $type .'_max_per_group'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of members per group for the @type group type', array('@type' => $type)),
      '#default_value' =>  variable_get('og_global_limits_' . $type .'_max_per_group', 5),
      '#description' => t('Set the maximum number of users that may join this group type.  Use 0 if you do not want to restrict group membership.'),
      '#required' => TRUE,
    );
    $form['og_global_limts_' . $type]['og_global_limits_' . $type .'_max_groups'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of groups for the @type group type', array('@type' => $type)),
      '#default_value' => variable_get('og_global_limits_' . $type . '_max_groups', 10),
      '#description' => t('Set the maximum nuber of groups that a user may create for this group type.  Use 0 if you do now want to restrict group creation.'),
      '#required' => TRUE,
    );
  }

  return system_settings_form($form);
}

/**
 * og_global_limits_settings_validate - validate the system settings form
 */
function og_global_limits_settings_validate($form, &$form_state) {
  $types = variable_get('og_global_limits_group_types', array());

  foreach ($types as $type) {
    if (!is_numeric($form_state['values']['og_global_limits_' . $type . '_max_per_group']) || 
      $form_state['values']['og_global_limits_' . $type . '_max_per_group'] < 0
    ) {
      form_set_error('og_global_limits_' . $type .'_max_per_group', t('You must enter a positive integer for the max per group limit.'));
    }

    if (!is_numeric($form_state['values']['og_global_limits_' . $type . '_max_groups']) || 
      $form_state['values']['og_global_limits_' . $type . '_max_groups'] < 0
    ) {
      form_set_error('og_global_limits_' . $type . '_max_groups', t('You must enter a positive integer for the max groups limit.'));
    }

    // just in case we've got some bozo that wants float limits, round up
    // only do this if we didn't have any errors
    if (sizeof(form_get_errors()) == 0) {
      $form_state['values']['og_global_limits_' . $type . '_max_per_group'] = ceil($form_state['values']['og_global_limits_' . $type . '_max_per_group']);
      $form_state['values']['og_global_limits_' . $type . '_max_groups'] = ceil($form_state['values']['og_global_limits_' . $type . '_max_groups']);
    }
  }
}

