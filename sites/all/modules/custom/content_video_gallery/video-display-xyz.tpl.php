<?php
 $module_path = drupal_get_path('module', 'content_video_gallery');
 $file_path = base_path().$module_path;
 ?>

<div class="gallery">
  <div class="gallery-search">
           <?php print drupal_get_form('video_search_form') ?>
   </div>

<?php
 
 $node_data = node_load($node_id);
 $youtube_path =  $node_data->field_youtube_video[0][embed];
$youtube_thumb_image = $node_data->field_youtube_video[0][data][thumbnail][url];
   
?>

<div class="video-gallery">
          <h6><?php print $node_data->title ?></h6>
          <div id="mediaplayer">JW Player goes here</div>
			<script type="text/javascript">
				jwplayer("mediaplayer").setup({
					flashplayer: "<?php print  $file_path ?>/js/player.swf",
					file: "<?php echo $youtube_path ?>",
					image: "<?php echo $youtube_thumb_image ?>",
					width:378
				});
			</script>
          </div>
	
	

	

<?php

 
 //print_r($node_data->taxonomy);
 foreach($node_data->taxonomy as $key => $value)
 {
	 $taxonomy_id = $value->tid;
	 $taxonomy_name = $value->name;

 }
 
 $sql ="SELECT DISTINCT tn.nid FROM term_node AS tn JOIN node AS n ON n.nid= tn.nid WHERE tn.nid !=$node_id AND tn.tid= $taxonomy_id ORDER BY n.created DESC";
  
  
  $res = db_query( $sql);
  while($row = db_fetch_object($res))
  {
	  
	  $nd= node_load($row->nid);
	  $cat_name= $taxonomy_name;
	  $nd_title = $nd->title;
	  $nd_nid = $nd->nid;
	  $nd_created = date("F d, Y", $nd->created);
	  $yt_thumb_image = $nd->field_youtube_video[0][data][thumbnail][url];

	  $options = array(
								   'attributes' => array(),
								   'html' => TRUE,
								   'query' => array('vid' => $nd_nid),
							   );

			$path = "video-gallery/view";
			$image = '<img src="'.$yt_thumb_image.'"  width="96" height="72"  />';
				 
				

	  echo "<div class='video-thum'>";
      echo "<div class='video-thum-img'>".l($image, $path, $options)."</div>";
      echo "<div class='video-thum-text'>";
      echo "<p><span>". $nd_title."</span></p>";
      echo "<p>Posted on:- ".$nd_created."</p>";
      echo " <p>Category:- ". $cat_name."</p>";
      echo "</div>";
      echo "</div>";
  }


 $options = array(
								   'attributes' => array(),
								   'html' => TRUE,
								   //'query' => array('vid' => $nd_nid),
							   );
$path = "video-gallery";
$image = '<img src="'.$file_path.'/images/view-all-video-button.jpg"  />';
 
?>

<div class="view-all-video"><?php echo l($image, $path, $options) ?></div>
</div>	
	