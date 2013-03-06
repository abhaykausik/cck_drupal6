<?php


/**
 * @file
 * views_handler_filter_relationship_type for Views integration.
 */

/**
 * Field handler to filter according to the relationship type.
 */
class views_handler_filter_relationship_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Relationship type');
      $relation_types = friendlist_api_relation_types_load_all();
      $options = array();
      foreach ($relation_types as $rt) {
        $options[ $rt->rtid ] = t( $rt->name );
      }
      $this->value_options = $options;
    }
  }
}

