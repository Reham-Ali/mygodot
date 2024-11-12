class A:
	@warning_ignore("unused_private_class_variable")
	var __a
	static var __b

	func __c():
		pass

	static func __d():
		pass

class B:
	var a = A.new()
	var b = a.__a
	@warning_ignore("accessing_private_member")
	var c = a.__a
	@warning_ignore("accessing_private_member") var d = a.__a
	var e = a.__a

	var f = A.__b
	@warning_ignore("accessing_private_member")
	var g = A.__b
	@warning_ignore("accessing_private_member") var h = A.__b
	var i = A.__b

	func __init():
		a.__c()
		@warning_ignore("calling_private_method")
		a.__c()
		@warning_ignore("calling_private_method") a.__c()
		a.__c()

		A.__d()
		@warning_ignore("calling_private_method")
		A.__d()
		@warning_ignore("calling_private_method") A.__d()
		A.__d()

	var j = a.__c()
	@warning_ignore("calling_private_method")
	var k = a.__c()
	@warning_ignore("calling_private_method") var l = a.__c()
	var m = a.__c()

	var n = A.__d()
	@warning_ignore("calling_private_method")
	var o = A.__d()
	@warning_ignore("calling_private_method") var p = A.__d()
	var q = A.__d()

func test():
	pass
