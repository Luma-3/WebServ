/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tic_tac_toe.js                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:35:21 by anthony           #+#    #+#             */
/*   Updated: 2024/11/06 14:35:24 by anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

const board = document.getElementById("board");
const message = document.getElementById("message");
let currentPlayer = "X";
let gameActive = true;
let boardState = ["", "", "", "", "", "", "", "", ""];

function checkWinner() {
	const winningCombinations = [
		[0, 1, 2], [3, 4, 5], [6, 7, 8],
		[0, 3, 6], [1, 4, 7], [2, 5, 8],
		[0, 4, 8], [2, 4, 6]
	];
	for (const combination of winningCombinations) {
		const [a, b, c] = combination;
		if (boardState[a] && boardState[a] === boardState[b] && boardState[a] === boardState[c]) {
			return boardState[a];
		}
	}
	return boardState.includes("") ? null : "Tie";
}

function handleClick(event) {
	const cell = event.target;
	const index = Array.from(board.children).indexOf(cell);

	if (boardState[index] || !gameActive) return;

	boardState[index] = currentPlayer;
	cell.textContent = currentPlayer;

	const winner = checkWinner();
	if (winner) {
		gameActive = false;
		message.textContent = winner === "Tie" ? "Égalité !" : `${winner} a gagné !`;
	} else {
		currentPlayer = currentPlayer === "X" ? "O" : "X";
	}
}

function initBoard() {
	board.innerHTML = "";
	boardState = ["", "", "", "", "", "", "", "", ""];
	gameActive = true;
	currentPlayer = "X";
	message.textContent = "";
	for (let i = 0; i < 9; i++) {
		const cell = document.createElement("div");
		cell.classList.add("cell");
		cell.addEventListener("click", handleClick);
		board.appendChild(cell);
	}
}

initBoard();