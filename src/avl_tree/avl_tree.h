#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "binary_search_tree.h"
#include <iostream>

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class avl_tree : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

protected:

    struct avl_node : binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node
    {
        size_t height = 0;
    };

protected:

    class avl_insertion_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::bin_insertion_template_method
    {

    private:

        size_t get_node_size() const override;

    private:

        avl_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:

        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive) override;

    public:

        explicit avl_insertion_template_method(
                avl_tree<tkey, tvalue, tkey_comparer> *tree);

    };

    class avl_removing_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::bin_removing_template_method
    {

    private:

        avl_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:

        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive) override;

    public:

        explicit avl_removing_template_method(
                avl_tree<tkey, tvalue, tkey_comparer> *tree);

    };

public:

    explicit avl_tree(
            memory *allocator_binary_search_tree = nullptr,
            logger *logger_tree = nullptr);

    ~avl_tree() final = default;

private:

    int get_height(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *node);

    void fix_height(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node **subtree_address);

    int balance_factor(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *subtree_address);

public:

    avl_tree(
            const avl_tree<tkey, tvalue, tkey_comparer> &target_copy);

    avl_tree& operator=(
            const avl_tree<tkey, tvalue, tkey_comparer> &target_copy);

    avl_tree(
            avl_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept;

    avl_tree& operator=(
            avl_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept;

private:

    void balance(
            typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
            std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive);

};

//begin region for balance node

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
int avl_tree<tkey, tvalue, tkey_comparer>::get_height(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *node)
{
    return node == nullptr ? 0 : (reinterpret_cast<avl_node*>(node))->height;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::fix_height(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node **subtree_address)
{
    if (subtree_address != nullptr &&
        *subtree_address != nullptr)
    {
        int height_left = (*subtree_address != nullptr) ? (get_height((*subtree_address)->left_subtree)) : 0;
        int height_right = (*subtree_address != nullptr) ? (get_height((*subtree_address)->right_subtree)) : 0;

        reinterpret_cast<avl_node*>(*subtree_address)->height = std::max(height_left, height_right) + 1;
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
int avl_tree<tkey, tvalue, tkey_comparer>::balance_factor(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *subtree_address)
{
    int height_left = get_height(subtree_address->left_subtree);
    int height_right = get_height(subtree_address->right_subtree);

    return height_left - height_right;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::balance(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node **parent = nullptr;

    if (!path_to_subtree_root_exclusive.empty())
    {
        parent = path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }

    fix_height(&subtree_root_address);
    int bal_factor = balance_factor(subtree_root_address);

    if (bal_factor == 2)
    {
        if (balance_factor(subtree_root_address->left_subtree) < 0)
        {
            this->left_rotate(&(subtree_root_address->left_subtree), &subtree_root_address);
            fix_height(&(subtree_root_address->left_subtree->left_subtree));
            fix_height(&(subtree_root_address->left_subtree->right_subtree));
            fix_height(&(subtree_root_address->left_subtree));
        }

        this->right_rotate(&subtree_root_address, parent);
        fix_height(&(subtree_root_address->right_subtree));
        fix_height(&(subtree_root_address->left_subtree));
        fix_height(&subtree_root_address);
    }

    if (bal_factor == -2)
    {
        if (balance_factor(subtree_root_address->right_subtree) > 0)
        {
            this->right_rotate(&(subtree_root_address->right_subtree), &subtree_root_address);
            fix_height(&(subtree_root_address->right_subtree->right_subtree));
            fix_height(&(subtree_root_address->right_subtree->left_subtree));
            fix_height(&(subtree_root_address->right_subtree));
        }

        this->left_rotate(&subtree_root_address, parent);
        fix_height(&(subtree_root_address->right_subtree));
        fix_height(&(subtree_root_address->left_subtree));
        fix_height(&subtree_root_address);
    }

    if (parent != nullptr)
    {
        path_to_subtree_root_exclusive.push(parent);
    }
}

//end region for balance node


//begin region insertion template method

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::get_node_size() const
{
    return sizeof(avl_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::after_insert_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *current_node = subtree_root_address;

    while (!path_to_subtree_root_exclusive.empty())
    {
        _tree->balance(current_node, path_to_subtree_root_exclusive);
        current_node = *path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_insertion_template_method::avl_insertion_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree) :
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::bin_insertion_template_method(tree)
{

}

//end region insertion template method


//begin region removing template method

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void avl_tree<tkey, tvalue, tkey_comparer>::avl_removing_template_method::after_remove_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive)
{
    typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *current_node = subtree_root_address;

    while (!path_to_subtree_root_exclusive.empty())
    {
        _tree->balance(current_node, path_to_subtree_root_exclusive);
        current_node = *path_to_subtree_root_exclusive.top();
        path_to_subtree_root_exclusive.pop();
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_removing_template_method::avl_removing_template_method(
        avl_tree<tkey, tvalue, tkey_comparer> *tree) :
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::bin_removing_template_method(tree)
{

}

//end region removing template method


//begin region constructor

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(
        memory *allocator_binary_search_tree,
        logger *logger_tree) :
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                new typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_find_template_method(this),
                new avl_insertion_template_method(this),
                new avl_removing_template_method(this),
                allocator_binary_search_tree,
                logger_tree,
                nullptr)
{

}

//end region constructor


//begin region move/copy semantics

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(
        const avl_tree<tkey, tvalue, tkey_comparer> &target_copy) :
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                this->_find,
                this->_insertion,
                this->_removing,
                this->_allocator,
                this->_logger,
                this->_root)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>& avl_tree<tkey, tvalue, tkey_comparer>::operator=(
        const avl_tree<tkey, tvalue, tkey_comparer> &target_copy)
{
    *this = target_copy;

    return *this;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>::avl_tree(
        avl_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept
{
    *this = std::move(target_copy);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
avl_tree<tkey, tvalue, tkey_comparer>& avl_tree<tkey, tvalue, tkey_comparer>::operator=(
        avl_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept
{
    *this = std::move(target_copy);

    return *this;
}

//end region move/copy semantics

#endif // AVL_TREE_H