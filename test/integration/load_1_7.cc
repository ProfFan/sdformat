#include <gtest/gtest.h>
#include "sdf/sdf.hh"

#include "test_config.h"

/////////////////////////////////////////////////
/// Test conversion using the parser sdf string converter interface.
TEST(SDF1_7, Load1_7World)
{
  // The gravity and magnetic_field in 1.5 format
  std::string xmlString = R"(
<?xml version="1.0" ?>
<sdf version="1.7">
  <world name="1_7_world_loaded">
  </world>
</sdf>)";

  sdf::SDFPtr sdf(new sdf::SDF());
  sdf->SetFromString(xmlString);

  ASSERT_TRUE(sdf->Root() != NULL);
  EXPECT_EQ(sdf->Root()->GetName(), "sdf");

  sdf::ElementPtr worldElem = sdf->Root()->GetElement("world");
  ASSERT_TRUE(worldElem != NULL);
  EXPECT_EQ(worldElem->Get<std::string>("name"), "1_7_world_loaded");

}
