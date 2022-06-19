for $x in catalog/array
 where $x/PASSED_NUMBER_FULL>80
 order by $x/global_id
 return $x/ID
