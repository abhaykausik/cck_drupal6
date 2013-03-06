<?php

/**
 * Crates a deep clone of an object, in the PHP way.
 *
 * @param object $obj
 * @return object
 */
function og_schedule_deep_clone($obj) {
  return unserialize(serialize($obj));
}

/**
 * Returns a normalized form of the session array.
 *
 * This form contains a lot of redundancy, but this makes easier the rendering.
 *
 * Speed: O(n^3)
 *
 * @param array $sessions
 *  Array of sessions. You normally want to pass OGScheduleDAO::getAllSessionNodes()'s return value.
 * @return array
 */
function _og_schedule_api_normalize_session_array($sessions) {
  $ret = array();

  foreach($sessions as $s) {
    foreach($s->dates as $d) {
      foreach($s->tracks as $t) {
        $obj = new stdClass();
        $obj->nid = $s->nid;
        $obj->title = $s->title;
        $obj->date = og_schedule_deep_clone($d);
        $obj->track = og_schedule_deep_clone($t);

        $cmp = _og_schedule_api_dates_cmp($obj->date->from, $obj->date->to);
        if($cmp === 0)
          $ret []= og_schedule_deep_clone($obj);
        else {
          $diff = _og_schedule_api_dates_diff($obj->date->from, $obj->date->to);
          for($i=0; $i<=$diff; ++$i) {
            $obj2 = og_schedule_deep_clone($obj);
            // TODO make the start|end times configurable
            if($i == 0) {
              $obj2->date->to = og_schedule_deep_clone($obj2->date->from);
              $obj2->date->to->hour = '23';
              $obj2->date->to->minute = '59';
            } else if($i == $diff) {
              $obj2->date->from = og_schedule_deep_clone($obj2->date->to);
              $obj2->date->from->hour = '00';
              $obj2->date->from->minute = '00';
            } else {
              $date = split('-', date('Y-m-d', strtotime("+$i days", strtotime($obj->date->from->year.'-'.$obj->date->from->month.'-'.$obj->date->from->day))));
              $obj2->date->from->year = $obj2->date->to->year = $date[0];
              $obj2->date->from->month = $obj2->date->to->month = $date[1];
              $obj2->date->from->day = $obj2->date->to->day = $date[2];
              $obj2->date->from->hour = '00';
              $obj2->date->from->minute = '00';
              $obj2->date->to->hour = '23';
              $obj2->date->to->minute = '59';
            }
            $ret []= og_schedule_deep_clone($obj2);
          }
        }
      }
    }
  }

  return $ret;
}

/**
 * Decomposes an array of the sessions by days.
 *
 * @param array $sessions
 * @return array
 */
function _og_schedule_api_decompose_by_days($sessions) {
  $ret = array();
  foreach($sessions as $s) {
    $key = $s->date->from->year . '-' . $s->date->from->month . '-' . $s->date->from->day;
    if(!isset($ret[$key])) $ret[$key] = array();
    $ret[$key] []= $s;
  }
  return $ret;
}

/**
 * Decomposes an array of the sessions by tracks.
 *
 * @param array $sessions
 * @return array
 */
function _og_schedule_api_decompose_by_tracks($sessions) {
  $ret = array();
  foreach($sessions as $s) {
    if(!isset($ret[$s->track])) $ret[$s->track] = array();
    $ret[$s->track] []= $s;
  }
  return $ret;
}

/**
 * Compares two time objects like strcmp().
 *
 * @param stdClass $time0
 * @param stdClass $time1
 * @return int
 */
function _og_schedule_api_times_cmp($time0, $time1) {
  if($time0->hour != $time1->hour)
    return $time0->hour < $time1->hour ? -1 : 1;
  if($time0->minute != $time1->minute)
    return $time0->minute < $time1->minute ? -1 : 1;
  return 0;
}

/**
 * Compare two date objects like strcmp().
 *
 * @param stdClass $date0
 * @param stdClass $date1
 * @return int
 */
function _og_schedule_api_dates_cmp($date0, $date1) {
  if($date0->year != $date1->year)
    return $date0->year < $date1->year ? -1 : 1;
  if($date0->month != $date1->month)
    return $date0->month < $date1->month ? -1 : 1;
  if($date0->day != $date1->day)
    return $date0->day < $date1->day ? -1 : 1;
  return 0;
}

/**
 * Calculate the difference in days between two dates.
 *
 * @param stdClass $date0
 * @param stdClass $date1
 * @return int
 */
function _og_schedule_api_dates_diff($date0, $date1) {
  return (int)abs((
      strtotime($date0->year.'-'.$date0->month.'-'.$date0->day) -
      strtotime($date1->year.'-'.$date1->month.'-'.$date1->day)
    ) / (60*60*24));
}

/**
 * Returns the discrete times of the sessions.
 *
 * Note: this function does not do anything with years, months and days.
 *
 * @param array $sessions
 * @return array
 */
function _og_schedule_api_get_discrete_times($sessions) {
  $times = array();
  foreach($sessions as $s) {
    $times []= _og_schedule_api_dateobj_time_to_string($s->date->from);
    $times []= _og_schedule_api_dateobj_time_to_string($s->date->to);
  }
  return array_unique($times);
}

/**
 * Converts a "dateobj" type stdClass to a string.
 *
 * Note: this function only converts hours and minutes.
 *
 * @param stdClass $dateobj
 * @return string
 */
function _og_schedule_api_dateobj_time_to_string($dateobj) {
  return (strlen($dateobj->hour) < 2 ? '0'.$dateobj->hour : $dateobj->hour) . ':' .
    (strlen($dateobj->minute) < 2 ? '0'.$dateobj->minute : $dateobj->minute );
}

/**
 * Counts how many sessions are in the specified interval.
 *
 * @param array $sessions
 * @param string $from
 * @param string $to
 * @return array
 */
function _og_schedule_api_get_sessions_in_interval($sessions, $from, $to) {
  $ret = array();
  $from = strtotime($from);
  $to = strtotime($to);
  foreach($sessions as $s) {
    $session_from = strtotime($s->date->from->hour . ':' . $s->date->from->minute);
    $session_to = strtotime($s->date->to->hour . ':' . $s->date->to->minute);
    if(
        ($session_from == $from) ||
        ($session_to == $to) ||
        ($session_from < $from && $session_to > $to)
    )
      $ret []= $s;
  }
  return $ret;
}

function _og_schedule_api_optimize_table_vertically($nid, $table, $skip_col = 0) {
  $track_objects = og_schedule_get_dao()->getChildTrackNodeObjects($nid);
  $tracks = array();
  foreach($track_objects as $t) $tracks []= $t->nid;
  $table_keys = array_keys($table);
  $prev_key = array_shift($table_keys);
  foreach($table_keys as $key) {
    foreach($tracks as $t) {
      if(isset($table[$prev_key][$t]) && isset($table[$key][$t]) && ($table[$prev_key][$t]->nid == $table[$key][$t]->nid)) {
        $item = new stdClass;
        $item->nid = $table[$prev_key][$t]->nid;
        $item->title = '';
        $item->dummy = TRUE;
        $table[$key][$t] = $item;
      }
    }
    $prev_key = $key;
  }
  return $table;
}

function _og_schedule_api_optimize_table_horizontally($nid, $table, $skip_col = 0) {
  return $table;
}

/**
 * Assembles a raw schedule table of a group.
 *
 * @param int $gid
 * @return array
 */
function _og_schedule_api_assemble_table($gid) {
  $tables = array();
  $dao = og_schedule_get_dao();
  $sessions = _og_schedule_api_decompose_by_days(_og_schedule_api_normalize_session_array($dao->getAllSessionNodes($gid)));

  foreach($sessions as $k=>$v) { // days
    $table = array();
    // preprocessing
    $intervals = _og_schedule_api_get_discrete_times($v);
    sort($intervals);
    $interval_pairs = _og_schedule_api_generate_intervals_pairs($intervals);

    foreach($interval_pairs as $i) {
      $row = array('interval' => $i[0].'-'.$i[1]);
      $rowsessions = _og_schedule_api_get_sessions_in_interval($v, $i[0], $i[1]);
      foreach($rowsessions as $rs) {
        $row[$rs->track] = $rs;
      }
      $table []= $row;
    }

    // postprocessing
    $table = _og_schedule_api_optimize_table_vertically($gid, $table);
    $table = _og_schedule_api_optimize_table_horizontally($gid, $table);
    $tables [$k]= $table;
  }
  return array_reverse($tables);
}

/**
 * Generates an array of pairs from a list of intervals.
 *
 * @param array $intervals
 * @return array
 */
function _og_schedule_api_generate_intervals_pairs($intervals) {
  switch(count($intervals)) {
    case 0:
      return array();
      break;
    case 1:
      $i = array_shift($intervals);
      return array(array($i, $i));
      break;
    default:
      $pairs = array();
      $i_prev = array_shift($intervals);
      foreach($intervals as $i) {
        $pairs []= array($i_prev, $i);
        $i_prev = $i;
      }
      return $pairs;
  }
}

function _og_schedule_api_schedobj_date_to_string($schedobj) {
  return $schedobj->date->from->year . '-' . $schedobj->date->from->month . '-' . $schedobj->date->from->day . ' ' .
    $schedobj->date->from->hour . ':' . $schedobj->date->from->minute . '<br/>' .
    $schedobj->date->to->year . '-' . $schedobj->date->to->month . '-' . $schedobj->date->to->day . ' ' .
    $schedobj->date->to->hour . ':' . $schedobj->date->to->minute;
}

function _og_schedule_api_schedobj_to_string($schedobj) {
  return l($schedobj->title, 'node/'.$schedobj->nid) //. ' (' . $schedobj->nid . ')'
    //. '<br/>' . _og_schedule_api_schedobj_date_to_string($schedobj)
  ;
}