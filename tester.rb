#!/usr/bin/env ruby
require 'tk'

def insert_colored_text(text, color)
	$text.configure :state => 'normal'
	$text.delete '1.0', 'end'
	$text.insert 'end', text	
	$text.configure :state => 'disabled'
	$status.configure :readonlybackground => color
end

def run_cmd(cmd)
	out = %x[#{cmd}] 
	insert_colored_text(out, $?.exitstatus != 0 ? 'red' : 'green')
end

root = TkRoot.new() {
	title "Tester v0.666"
}

TkGrid.rowconfigure root, 1, :weight => 1
TkGrid.columnconfigure root, 0, :weight => 1

$status = TkEntry.new(root) {
	state "readonly"
	font TkFont.new('Terminus 11')
	grid(:row => 0, :column => 0, :columnspan => 3, :sticky => 'we')
}

$text = TkText.new(root) {
	wrap "word"
	state "disabled"
	font TkFont.new('Terminus 11')
	grid(:row => 1, :column => 0, :columnspan => 2, :sticky => 'nwse')
}

$scroll = TkScrollbar.new(root) {
	grid(:row => 1, :column => 2, :sticky => 'nse')
	command proc { |*args|
		$text.yview *args
	}
}

$text.yscrollcommand proc { |f,l|
	$scroll.set f, l
}

button = TkButton.new(root) {
	text 'RUN'
	font TkFont.new('DejaVuSans 16 bold')
	grid(:row => 2, :column => 0, :sticky => 'we')
	command proc {
		run_cmd('make test 2>&1')
	}
}

button2 = TkButton.new(root) {
	text 'CLEAN'
	font TkFont.new('DejaVuSans 16 bold')
	grid(:row => 2, :column => 1, :columnspan => 2, :sticky => 'we')
	command proc {
		run_cmd('make clean 2>&1')
	}
}

Tk.mainloop()
