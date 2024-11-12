class A:
	@warning_ignore("unused_private_class_variable")
	var _a
	static var _b

class B extends A:
	var a = A.new()
	var b = a._a
	@warning_ignore("accessing_private_member") var c = a._a
	var d = a._a
	var e = A._b
	@warning_ignore("accessing_private_member") var f = A._b
	var g = A._b

class C:
	var k = A.new()
	var l = k._a
	@warning_ignore("accessing_private_member") var m = k._a
	var n = k._a
	var o = A._b
	@warning_ignore("accessing_private_member") var p = A._b
	var q = A._b

func test():
	pass
