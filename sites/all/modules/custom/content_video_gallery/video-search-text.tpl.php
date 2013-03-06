<?php
	global $_domain;
		
	$domain_id = strtolower($_domain['domain_id']);

	 

	if($domain_id==0)
	{
		$sql_count = "SELECT count(*) FROM node AS n JOIN term_node AS tn ON tn.nid=n.nid JOIN domain_source AS ds ON ds.nid=n.nid WHERE n.type='video_gallery' AND (LOWER(n.title) LIKE '% $data_id %' OR LOWER(n.title) LIKE '$data_id %' OR LOWER(n.title) LIKE '% $data_id' OR LOWER(n.title) LIKE '$data_id') ORDER BY n.created DESC";
	}
	else
	{
		$sql_count = "SELECT count(*) FROM node AS n JOIN term_node AS tn ON tn.nid=n.nid JOIN domain_source AS ds ON ds.nid=n.nid WHERE n.type='video_gallery' AND ds.domain_id=$domain_id AND (LOWER(n.title) LIKE '% $data_id %' OR LOWER(n.title) LIKE '$data_id %' OR LOWER(n.title) LIKE '% $data_id' OR LOWER(n.title) LIKE '$data_id') ORDER BY n.created DESC";
	}

	

   $rec = db_result(db_query($sql_count));

  
   
   if($rec==0)
   {
     $output .= "<div class='no-record'>No Record Found</div>";
   }
   else
   {
	       if($domain_id==0)
			{
				  $sql= "SELECT n.nid FROM node AS n JOIN term_node AS tn ON tn.nid=n.nid JOIN domain_source AS ds ON ds.nid=n.nid WHERE n.type='video_gallery' AND (LOWER(n.title) LIKE '% $data_id %' OR LOWER(n.title) LIKE '$data_id %' OR LOWER(n.title) LIKE '% $data_id' OR LOWER(n.title) LIKE '$data_id') ORDER BY n.created DESC";
			}
			else
			{
				  $sql= "SELECT n.nid FROM node AS n JOIN term_node AS tn ON tn.nid=n.nid JOIN domain_source AS ds ON ds.nid=n.nid WHERE n.type='video_gallery' AND ds.domain_id=$domain_id AND (LOWER(n.title) LIKE '% $data_id %' OR LOWER(n.title) LIKE '$data_id %' OR LOWER(n.title) LIKE '% $data_id' OR LOWER(n.title) LIKE '$data_id') ORDER BY n.created DESC";
			}
		   

		    $res = pager_query($sql,2,0,$sql_count);
		   
		   while($row = db_fetch_object($res))
		   {
	  
			  $nd= node_load($row->nid);
			  $nd_title = $nd->title;
			  $nd_nid = $nd->nid;
			  $nd_created = date("F d, Y", $nd->created);

			   foreach( $nd->taxonomy as $key => $value)
				 {
					 $taxonomy_id = $value->tid;
					 $taxonomy_name = $value->name;

				 }
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
			  echo " <p>Category:- ". $taxonomy_name."</p>";
			  echo "</div>";
			  echo "</div>";
		  }

		   echo "<div class='pagination-box'>".theme('pager', NULL, 2, 0)."</div>";
  }
  
?>