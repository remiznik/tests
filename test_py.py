import unittest

class DoublyLinkedList(object):

    class Node(object):

        def __init__(sefl):

            self._value = None
            self._prev = None
            self._next = None 

    def __init__(self):

        self._head = Node()        
        
    def append(self, value):

        n = Node()
        node = self._head
        curr_node = self._head
        while node is not None:
            curr_node = node
            node = node._next
        


    def take(self, index):

        raise Exception

    def empty(self):
        return self._isEmpty


class DoublyLinkedListTest(unittest.TestCase):
    
    def test_create(self):
        try:
            DoublyLinkedList()
        except Exception as e:
            self.fail("Should create!")

    def test_after_create_should_be_empty(self):

        l = DoublyLinkedList()

        self.assertFalse(l.empty())

    def test_after_append_should_be_not_empty(self):

        l = DoublyLinkedList()
        l.append(object())

        self.assertTrue(l.empty())

    def test_empty_list_should_return_None(self):

        l = DoublyLinkedList()

        with self.assertRaises(Exception):
            l.take(0)

    def test_after_append_list_should_can_return(self):

        l = DoublyLinkedList()

        obj = object()

        l.append(obj)

        self.assertEqual(obj, l.take(0))

if __name__ == '__main__':
    unittest.main()
