/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:21:12 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/06 15:21:25 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main()
{
	std::cout << "Hello World!" << std::endl;
	return 0;
}

// #include <cstdlib>
// #include <iostream>
// #include <vector>

// void test(int a)
// {
// 	if (a = 5) { // Erreur potentielle : affectation au lieu de comparaison
// 		std::cout << "a vaut 5" << std::endl;
// 	}
// }

// int main()
// {
// 	int				 x = NULL; // Utilisation de NULL au lieu de nullptr
// 	std::vector<int> vec;
// 	for (int i = 0; i < 10; i++) { // 10 est un nombre magique
// 		vec.push_back(i);
// 	}

// 	int *ptr = (int *)malloc(
// 		sizeof(int)); // Utilisation de malloc au lieu de new, non recommandé
// 	*ptr = 10;		  // 10 est un nombre magique

// 	std::cout << "Valeur de ptr : " << *ptr << std::endl;
// 	free(ptr); // Utilisation de free au lieu de delete

// 	return 0;
// }
