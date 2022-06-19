for $x in catalog/array
 where $x/ID<40
 order by $x/global_id
 return $x/RegistrationLicenseNumber
