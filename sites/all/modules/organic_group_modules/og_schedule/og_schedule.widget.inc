<?php

/**
 * Returns the schedule page.
 *
 * @todo Implement some hardcore caching system.
 *
 * @param int $nid
 * @return string
 */
function og_schedule_show_schedule($nid) {
  $output = '';
  $tables = _og_schedule_api_assemble_table($nid);
  $tracks = og_schedule_get_dao()->getChildTrackNodeObjects($nid);
  foreach($tables as $k=>$v) {
    $output .= theme_og_schedule_day($k);
    $rows = array();
    $header = array();
    foreach($tracks as $t) {
      $header []= array('data' => $t->title, 'class' => 'header-'.$t->nid);
    }
    foreach($v as $i) {
      $row = array(array('data' => $i['interval']));
      unset($i['interval']);
      foreach($header as $h) {
        $nid = split('-', $h['class']);
        $nid = $nid[1];
        if(isset($i[$nid])) {
          $row []= array('data' => _og_schedule_api_schedobj_to_string($i[$nid]), 'class' => 'item-'.$i[$nid]->nid);
        } else {
          $row []= array('data' => '&nbsp;', 'class' => 'empty-cell');
        }
      }
      $rows []= $row;
    }
    array_unshift($header, array('data'=>'&nbsp;'));
    $output .= theme('table', $header, $rows);
  }
  return $output;
}

function theme_og_schedule_day($day) {
  return '<h3>'.$day.'</h3>';
}