<?php

interface OGScheduleDAO {
  public function getChildSessionNodes($nid);
  public function getChildTrackNodeObjects($nid);
  public function getTitle($nid);
  public function getTracks($nid);
  public function getDates($nid);
  public function getFull($nid);
  public function getAllSessionNodes($nid);
}

/**
 * This is a dispatcher for OGScheduleDAO.
 *
 * @staticvar stdClass $dao
 * @return stdClass
 */
function og_schedule_get_dao() {
  static $dao = NULL;
  if($dao === NULL) {
    $dao = new OGScheduleClassicDAO(); // TODO make this a bit more modular
  }
  return $dao;
}

class OGScheduleClassicDAO implements OGScheduleDAO {

  /**
   * Cache for node objects.
   *
   * @var array 
   */
  protected $nodes = array();

  /**
   * Track content type name.
   *
   * @var string
   */
  protected $track_type;

  /**
   * Session content type name.
   *
   * @var string
   */
  protected $session_type;

  /**
   * Date CCK name.
   *
   * @var string
   */
  protected $date_ckk;

  /**
   * Node reference CCK name.
   *
   * @var string
   */
  protected $ref_cck;

  public function __construct() {
    $this->track_type = variable_get('og_schedule_track_type', NULL);
    $this->session_type = variable_get('og_schedule_session_type', NULL);
    $this->date_ckk = variable_get('og_schedule_session_date_cck', NULL);
    $this->ref_cck = variable_get('og_schedule_session_nodereference_cck', NULL);
  }

  protected function db_fetch_all($res) {
    $ret = array();
    while($row = db_fetch_array($res))
      $ret []= $row;
    return $ret;
  }

  protected function nodeGet($nid) {
    if(!isset($this->nodes[$nid])) {
      $node = node_load($nid);
      if($node !== FALSE)
        $this->nodes[$nid] = $node;
      else
        throw new InvalidArgumentException('The requested node does not exists!', 0);
    }
    return $this->nodes[$nid];
  }

  protected function getAllChildNodes($nid) {
    static $cache = array();
    if(!isset($cache[$nid])) {
      $arr = $this->db_fetch_all(db_query('SELECT nid FROM {og_ancestry} WHERE group_nid = %d ORDER BY nid', $nid));
      $ret = array();
      foreach($arr as $a) {
        $node = node_load($a['nid']);
        if($node->status)
          $ret []= $node;
      }
      $cache[$nid] = $ret;
    }
    return $cache[$nid];
  }
  
  public function getChildSessionNodes($nid) {
    $ret = array();
    foreach($this->getAllChildNodes($nid) as $node)
      if($node->type == $this->session_type)
        $ret []= $node->nid;
    return $ret;
  }

  public function getChildTrackNodeObjects($nid) {
    $ret = array();
    foreach($this->getAllChildNodes($nid) as $node)
      if($node->type == $this->track_type)
        $ret []= $node;
    return $ret;
  }
  
  public function getTitle($nid) {
    $node = $this->nodeGet($nid);
    return $node->title;
  }

  public function getTracks($nid) {
    $node = $this->nodeGet($nid);
    $ret = array();
    foreach($node->{$this->ref_cck} as $r) {
      $ret []= $r['nid'];
    }
    return $ret;
  }
  
  public function getDates($nid) {
    global $user;
    $timezone = 0;
    if (variable_get('configurable_timezones', 1) && $user->uid && strlen($user->timezone)) {
      $timezone = $user->timezone;
    }
    else {
      $timezone = variable_get('date_default_timezone', 0);
    }
    $node = $this->nodeGet($nid);
    $ret = array();
    foreach($node->{$this->date_ckk} as $d) {
      $obj = new stdClass();
      // timezone calculation
      $d['value'] = date('Y-m-d\TH:i', strtotime($d['value'])+$timezone);
      $d['value2'] = date('Y-m-d\TH:i', strtotime($d['value2'])+$timezone);

      // from
      $from = split('T', $d['value']);
      $from[0] = split('-', $from[0]);
      $from[1] = split(':', $from[1]);
      $obj->from = new stdClass();
      $obj->from->year = $from[0][0];
      $obj->from->month = $from[0][1];
      $obj->from->day = $from[0][2];
      $obj->from->hour = $from[1][0];
      $obj->from->minute = $from[1][1];
      // to
      $to = split('T', $d['value2']);
      $to[0] = split('-', $to[0]);
      $to[1] = split(':', $to[1]);
      $obj->to = new stdClass();
      $obj->to->year = $to[0][0];
      $obj->to->month = $to[0][1];
      $obj->to->day = $to[0][2];
      $obj->to->hour = $to[1][0];
      $obj->to->minute = $to[1][1];
      // other
      $obj->timezone = $d['timezone'];
      
      $ret []= $obj;
    }
    return $ret;
  }

  public function getFull($nid) {
    $ret = new stdClass();
    $ret->nid = $nid;
    $ret->title = $this->getTitle($nid);
    $ret->dates = $this->getDates($nid);
    $ret->tracks = $this->getTracks($nid);
    return $ret;
  }

  public function getAllSessionNodes($nid) {
    $ret = array();
    foreach($this->getChildSessionNodes($nid) as $cn) {
      $ret []= $this->getFull($cn);
    }
    return $ret;
  }
}