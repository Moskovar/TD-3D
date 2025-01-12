#pragma once

class Button
{
	public:
		Button() {}
		Button(char shortcut, bool showShortcut);

		char	shortcut = 0;
		bool	showShortcut = true;

	private:
	
};

