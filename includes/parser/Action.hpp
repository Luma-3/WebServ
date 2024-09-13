/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:59:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/13 14:07:35 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
#define ACTION_HPP

enum ActionType {
	SHIFT,
	REDUCE,
	ACCEPT,
	ERROR
};

class Action
{
  private:
	ActionType _type;
	int		   _next_states;
	void (*rule_func)();

  public:
	Action();
	Action(ActionType type, int next_states, void (*rule_func)());
	Action(const Action &src);
	Action &operator=(const Action &src);
	~Action();
};

#endif // ACTION_HPP