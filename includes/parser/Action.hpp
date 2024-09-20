/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:59:14 by jbrousse          #+#    #+#             */
/*   Updated: 2024/09/20 16:21:05 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
#define ACTION_HPP

#include "lexer/Token.hpp"
#include "Parser.hpp"

class Parser;

#define NB_ACTIONS 80

enum ActionType {
	SHIFT,
	REDUCE,
	ERROR
};

class Action
{
  private:
	ActionType _type;
	int		   _next_state;

	void (*_rule_func)(std::stack< Token * > &);

  public:
	Action();
	Action(ActionType type, int next_states,
		   void (*rule_func)(std::stack< Token * > &));
	Action(const Action &src);

	Action &operator=(const Action &src);

	void Shift(Token *token, std::stack< Token * > &stack,
			   Parser &parser) const;
	void Reduce(Token *token, std::stack< Token * > &stack,
				Parser &parser) const;
	int	 Execute(Token *token, std::stack< Token  *> &stack,
				 Parser &parser) const;
	~Action();
};

struct ActionEntry {
	Action		  action;
	int			  state;
	Terminal_Type terminal;
};

void R1(std::stack< Token * > &stack);
void R2(std::stack< Token * > &stack);
void R3(std::stack< Token * > &stack);
void R4(std::stack< Token * > &stack);
void R5(std::stack< Token * > &stack);
void R6(std::stack< Token * > &stack);

static const ActionEntry g_action[NB_ACTIONS] = {
	{Action(SHIFT,  1,  NULL), 0,	T_Server	},
	{Action(SHIFT,  2,  NULL), 1,	T_OBracket  },
	{Action(SHIFT,  29, NULL), 2,	 T_Location  },
	{Action(REDUCE, 0,  R1),	2,  T_CBracket	 },
	{Action(SHIFT,  3,  NULL), 2,	T_Port	  },
	{Action(SHIFT,  4,  NULL), 2,	T_Host	  },
	{Action(SHIFT,  5,  NULL), 2,	T_ErrorPage },
	{Action(SHIFT,  6,  NULL), 2,	T_AccessLog },
	{Action(SHIFT,  6,  NULL), 2,	T_ErroLog	 },
	{Action(SHIFT,  7,  NULL), 2,	T_Root	  },
	{Action(SHIFT,  8,  NULL), 2,	T_Index	   },
	{Action(SHIFT,  9,  NULL), 2,	T_Return	},
	{Action(SHIFT,  10, NULL), 2,	 T_AutoIndex },
	{Action(SHIFT,  11, NULL), 2,	 T_DenyMethod},
	{Action(SHIFT,  12, NULL), 3,	 T_Equal		},
	{Action(SHIFT,  13, NULL), 4,	 T_Equal		},
	{Action(SHIFT,  14, NULL), 5,	 T_Equal		},
	{Action(SHIFT,  15, NULL), 6,	 T_Equal		},
	{Action(SHIFT,  16, NULL), 7,	 T_Equal		},
	{Action(SHIFT,  17, NULL), 8,	 T_Equal		},
	{Action(SHIFT,  18, NULL), 9,	 T_Equal		},
	{Action(SHIFT,  19, NULL), 10, T_Equal	 },
	{Action(SHIFT,  20, NULL), 11, T_Equal	 },
	{Action(SHIFT,  21, NULL), 12, T_PortValue },
	{Action(SHIFT,  21, NULL), 13, T_HostValue },
	{Action(SHIFT,  22, NULL), 14, T_ErrorCode },
	{Action(SHIFT,  21, NULL), 15, T_FilePath	},
	{Action(SHIFT,  21, NULL), 16, T_Path		},
	{Action(SHIFT,  21, NULL), 17, T_FileName	},
	{Action(SHIFT,  23, NULL), 18, T_ErrorCode },
	{Action(SHIFT,  21, NULL), 19, T_Bool		},
	{Action(SHIFT,  24, NULL), 20, T_Method	  },
	{Action(REDUCE, 2,  R2),	21, T_Semi_Colon},
	{Action(SHIFT,  25, NULL), 22, T_Colone	  },
	{Action(SHIFT,  14, NULL), 22, T_Comma	 },
	{Action(SHIFT,  26, NULL), 23, T_Colone	  },
	{Action(REDUCE, 2,  R3),	24, T_Semi_Colon},
	{Action(SHIFT,  20, NULL), 24, T_Comma	 },
	{Action(SHIFT,  27, NULL), 25, T_FilePath	},
	{Action(SHIFT,  28, NULL), 26, T_FilePath	},
	{Action(SHIFT,  28, NULL), 26, T_Value	 },
	{Action(SHIFT,  28, NULL), 26, T_Path		},
	{Action(SHIFT,  28, NULL), 26, T_FileName	},
	{Action(REDUCE, 2,  R4),	27, T_Semi_Colon},
	{Action(REDUCE, 2,  R5),	28, T_Semi_Colon},
	{Action(SHIFT,  30, NULL), 29, T_Path		},
	{Action(SHIFT,  31, NULL), 30, T_OBracket	},
	{Action(REDUCE, 2,  R6),	31, T_CBracket  },
	{Action(SHIFT,  32, NULL), 31, T_ErrorPage },
	{Action(SHIFT,  33, NULL), 31, T_Root		},
	{Action(SHIFT,  34, NULL), 31, T_Index	 },
	{Action(SHIFT,  35, NULL), 31, T_Return	  },
	{Action(SHIFT,  36, NULL), 31, T_AutoIndex },
	{Action(SHIFT,  37, NULL), 31, T_DenyMethod},
	{Action(SHIFT,  38, NULL), 32, T_Equal	 },
	{Action(SHIFT,  39, NULL), 33, T_Equal	 },
	{Action(SHIFT,  40, NULL), 34, T_Equal	 },
	{Action(SHIFT,  41, NULL), 35, T_Equal	 },
	{Action(SHIFT,  42, NULL), 36, T_Equal	 },
	{Action(SHIFT,  43, NULL), 37, T_Equal	 },
	{Action(SHIFT,  44, NULL), 38, T_ErrorCode },
	{Action(SHIFT,  45, NULL), 39, T_FilePath	},
	{Action(SHIFT,  45, NULL), 40, T_FileName	},
	{Action(SHIFT,  46, NULL), 41, T_ErrorCode },
	{Action(SHIFT,  45, NULL), 42, T_Bool		},
	{Action(SHIFT,  47, NULL), 43, T_Method	  },
	{Action(SHIFT,  48, NULL), 44, T_Colone	  },
	{Action(SHIFT,  38, NULL), 44, T_Comma	 },
	{Action(REDUCE, 31, R2),	 45, T_Semi_Colon},
	{Action(SHIFT,  49, NULL), 46, T_Colone	  },
	{Action(REDUCE, 31, R3),	 47, T_Semi_Colon},
	{Action(SHIFT,  43, NULL), 47, T_Comma	 },
	{Action(SHIFT,  50, NULL), 48, T_FilePath	},
	{Action(SHIFT,  51, NULL), 49, T_FilePath	},
	{Action(SHIFT,  51, NULL), 49, T_Value	 },
	{Action(SHIFT,  51, NULL), 49, T_Path		},
	{Action(SHIFT,  51, NULL), 49, T_FileName	},
	{Action(REDUCE, 31, R4),	 50, T_Semi_Colon},
	{Action(REDUCE, 31, R5),	 51, T_Semi_Colon},
	{Action(ERROR,  -1, NULL), -1, T_None		},
};

#include "ToString.tpp"

#endif // ACTION_HPP