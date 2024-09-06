/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:25:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/06 16:24:09 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>


TEST(Parsing, truc)
{
	EXPECT_EQ(1, 1);
}


int main(void)
{
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}