#include <string>
#include <sstream>
#include <gtest/gtest.h>

#include "linear_sup_plugin.h"

using namespace sup;
typedef LinearSupPlugin::Nats Nats;

TEST(LinearSupPluginTest, Init)
{
  LinearSupPlugin sup_plugin(2);

  EXPECT_TRUE(sup_plugin.group_by(0).empty());
  EXPECT_TRUE(sup_plugin.group_by(1).empty());
}

TEST(LinearSupPluginTest, AppendNat)
{
  LinearSupPlugin sup_plugin(2);

  // default second argument
  sup_plugin.append_nat(0);
  EXPECT_EQ(1, sup_plugin.group_by(0).size());
  EXPECT_EQ(0, sup_plugin.group_by(1).size());

  sup_plugin.append_nat(0);
  EXPECT_EQ(2, sup_plugin.group_by(0).size());
  EXPECT_EQ(0, sup_plugin.group_by(1).size());

  sup_plugin.append_nat(1);
  EXPECT_EQ(2, sup_plugin.group_by(0).size());
  EXPECT_EQ(1, sup_plugin.group_by(1).size());

  // non-default second argument
  sup_plugin.append_nat(0, 7);
  EXPECT_EQ(3, sup_plugin.group_by(0).size());
  EXPECT_EQ(1, sup_plugin.group_by(1).size());

  EXPECT_EQ(0, sup_plugin.group_by(0).at(0));
  EXPECT_EQ(0, sup_plugin.group_by(0).at(1));
  EXPECT_EQ(7, sup_plugin.group_by(0).at(2));

  EXPECT_EQ(0, sup_plugin.group_by(0).at(0));

  sup_plugin.append_nat(1, 8);
  EXPECT_EQ(3, sup_plugin.group_by(0).size());
  EXPECT_EQ(2, sup_plugin.group_by(1).size());

  EXPECT_EQ(0, sup_plugin.group_by(0).at(0));
  EXPECT_EQ(0, sup_plugin.group_by(0).at(1));
  EXPECT_EQ(7, sup_plugin.group_by(0).at(2));

  EXPECT_EQ(0, sup_plugin.group_by(1).at(0));
  EXPECT_EQ(8, sup_plugin.group_by(1).at(1));
}

TEST(LinearSupPluginTest, Sup)
{
  LinearSupPlugin sup_plugin(8);
  
  EXPECT_TRUE(sup_plugin.group_by(0).empty());
  EXPECT_EQ(0, sup_plugin.sup(0, 1));

  sup_plugin.append_nat(0, 2);
  EXPECT_EQ(0, sup_plugin.sup(0, 1));
  EXPECT_EQ(2, sup_plugin.sup(0, 2));
  EXPECT_EQ(2, sup_plugin.sup(0, 3));

  sup_plugin.append_nat(1, 1);
  sup_plugin.append_nat(1, 2);
  EXPECT_EQ(1, sup_plugin.sup(1, 1));
  EXPECT_EQ(2, sup_plugin.sup(1, 2));
  EXPECT_EQ(2, sup_plugin.sup(1, 3));

  sup_plugin.append_nat(2, 1);
  sup_plugin.append_nat(2, 3);
  EXPECT_EQ(1, sup_plugin.sup(2, 1));
  EXPECT_EQ(1, sup_plugin.sup(2, 2));
  EXPECT_EQ(3, sup_plugin.sup(2, 3));
  EXPECT_EQ(3, sup_plugin.sup(2, 4));

  sup_plugin.append_nat(3, 3);
  sup_plugin.append_nat(3, 3);
  EXPECT_EQ(0, sup_plugin.sup(3, 1));
  EXPECT_EQ(0, sup_plugin.sup(3, 2));
  EXPECT_EQ(3, sup_plugin.sup(3, 3));
  EXPECT_EQ(3, sup_plugin.sup(3, 4));
  EXPECT_EQ(3, sup_plugin.sup(3, 5));

  sup_plugin.append_nat(4, 4);
  sup_plugin.append_nat(4, 4);
  sup_plugin.append_nat(4, 5);
  sup_plugin.append_nat(4, 5);
  EXPECT_EQ(0, sup_plugin.sup(4, 1));
  EXPECT_EQ(0, sup_plugin.sup(4, 2));
  EXPECT_EQ(0, sup_plugin.sup(4, 3));
  EXPECT_EQ(4, sup_plugin.sup(4, 4));
  EXPECT_EQ(5, sup_plugin.sup(4, 5));
  EXPECT_EQ(5, sup_plugin.sup(4, 6));

  sup_plugin.append_nat(5, 4);
  sup_plugin.append_nat(5, 4);
  sup_plugin.append_nat(5, 6);
  sup_plugin.append_nat(5, 6);
  EXPECT_EQ(0, sup_plugin.sup(5, 1));
  EXPECT_EQ(0, sup_plugin.sup(5, 2));
  EXPECT_EQ(0, sup_plugin.sup(5, 3));
  EXPECT_EQ(4, sup_plugin.sup(5, 4));
  EXPECT_EQ(4, sup_plugin.sup(5, 5));
  EXPECT_EQ(6, sup_plugin.sup(5, 6));
  EXPECT_EQ(6, sup_plugin.sup(5, 7));

  sup_plugin.append_nat(6, 3);
  sup_plugin.append_nat(6, 5);
  sup_plugin.append_nat(6, 6);
  sup_plugin.append_nat(6, 8);
  EXPECT_EQ(0, sup_plugin.sup(6, 1));
  EXPECT_EQ(0, sup_plugin.sup(6, 2));
  EXPECT_EQ(3, sup_plugin.sup(6, 3));
  EXPECT_EQ(3, sup_plugin.sup(6, 4));
  EXPECT_EQ(5, sup_plugin.sup(6, 5));
  EXPECT_EQ(6, sup_plugin.sup(6, 6));
  EXPECT_EQ(6, sup_plugin.sup(6, 7));
  EXPECT_EQ(8, sup_plugin.sup(6, 8));
  EXPECT_EQ(8, sup_plugin.sup(6, 9));

  sup_plugin.append_nat(7, 1);
  sup_plugin.append_nat(7, 3);
  sup_plugin.append_nat(7, 5);
  sup_plugin.append_nat(7, 6);
  sup_plugin.append_nat(7, 8);
  EXPECT_EQ(1, sup_plugin.sup(7, 1));
  EXPECT_EQ(1, sup_plugin.sup(7, 2));
  EXPECT_EQ(3, sup_plugin.sup(7, 3));
  EXPECT_EQ(3, sup_plugin.sup(7, 4));
  EXPECT_EQ(5, sup_plugin.sup(7, 5));
  EXPECT_EQ(6, sup_plugin.sup(7, 6));
  EXPECT_EQ(6, sup_plugin.sup(7, 7));
  EXPECT_EQ(8, sup_plugin.sup(7, 8));
  EXPECT_EQ(8, sup_plugin.sup(7, 9));
}

std::string to_string(const Nats& ns)
{
  std::stringstream out;
  for (Nats::const_iterator it = ns.begin(); it != ns.end(); it++) {
    out << *it << ' ';
  }
  return out.str();
}

TEST(LinearSupPluginTest, Update)
{
  LinearSupPlugin sup_plugin(1);

  sup_plugin.append_nat(0);
  sup_plugin.append_nat(0);
  sup_plugin.append_nat(0);
  sup_plugin.append_nat(0);
  sup_plugin.append_nat(0);

  EXPECT_TRUE(sup_plugin.update(0, 0, 4));
  EXPECT_EQ("0 0 0 0 4 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 4));
  EXPECT_EQ("0 0 0 4 4 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 4, 5));
  EXPECT_EQ("0 0 0 4 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 4, 5));
  EXPECT_EQ("0 0 0 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 1));
  EXPECT_EQ("0 0 1 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 1, 0));
  EXPECT_EQ("0 0 0 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 5));
  EXPECT_EQ("0 0 5 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 5, 4));
  EXPECT_EQ("0 0 4 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 2));
  EXPECT_EQ("0 2 4 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 4, 1));
  EXPECT_EQ("0 1 2 5 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 5, 6));
  EXPECT_EQ("0 1 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_FALSE(sup_plugin.update(0, 5, 5));
  EXPECT_EQ("0 1 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_FALSE(sup_plugin.update(0, 3, 5));
  EXPECT_EQ("0 1 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 1));
  EXPECT_EQ("1 1 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 1, 2));
  EXPECT_EQ("1 2 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 1, 0));
  EXPECT_EQ("0 2 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 0, 2));
  EXPECT_EQ("2 2 2 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 2, 3));
  EXPECT_EQ("2 2 3 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 2, 1));
  EXPECT_EQ("1 2 3 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 5, 4));
  EXPECT_EQ("1 2 3 4 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 6, 5));
  EXPECT_EQ("1 2 3 4 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 3, 0));
  EXPECT_EQ("0 1 2 4 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 2, 6));
  EXPECT_EQ("0 1 4 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 4, 0));
  EXPECT_EQ("0 0 1 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 1, 0));
  EXPECT_EQ("0 0 0 5 6 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 6, 0));
  EXPECT_EQ("0 0 0 0 5 ", to_string(sup_plugin.group_by(0)));

  EXPECT_TRUE(sup_plugin.update(0, 5, 0));
  EXPECT_EQ("0 0 0 0 0 ", to_string(sup_plugin.group_by(0)));
}
