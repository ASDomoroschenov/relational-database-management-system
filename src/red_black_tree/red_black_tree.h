#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "binary_search_tree.h"
#include <memory>

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class red_black_tree : public binary_search_tree<tkey, tvalue, tkey_comparer>
{

protected:

    enum class color_node
    {
        RED,
        BLACK
    };

    struct red_black_node : public binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node
    {
        color_node color;
    };

protected:

    class red_black_insertion_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::bin_insertion_template_method
    {

    private:

        size_t get_node_size() const override;

    private:

        red_black_tree<tkey, tvalue, tkey_comparer> *_tree;

    protected:

        void after_insert_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive) override;

    public:

        explicit red_black_insertion_template_method(
                red_black_tree<tkey, tvalue, tkey_comparer> *tree);
    };

    class red_black_removing_template_method : public binary_search_tree<tkey, tvalue, tkey_comparer>::bin_removing_template_method
    {

    private:

        red_black_tree<tkey, tvalue, tkey_comparer> *_tree;
        std::unique_ptr<red_black_node> _information_deleted_node;

    private:

        void get_info_deleted_node(
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *deleted_node,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive) override;

    protected:

        void after_remove_inner(
                tkey const &key,
                typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
                std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive) override;

    public:

        explicit red_black_removing_template_method(
                red_black_tree<tkey, tvalue, tkey_comparer> *tree);

        ~red_black_removing_template_method();
    };

public:

    explicit red_black_tree(
            memory *allocator_binary_search_tree = nullptr,
            logger *logger_tree = nullptr);

    ~red_black_tree() final = default;

private:

    color_node get_color(
            red_black_node *current_node);

public:

    red_black_tree(
            const red_black_tree<tkey, tvalue, tkey_comparer> &target_copy);

    red_black_tree& operator=(
            const red_black_tree<tkey, tvalue, tkey_comparer> &target_copy);

    red_black_tree(
            red_black_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept;

    red_black_tree& operator=(
            red_black_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept;
};

//begin region help functions

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
size_t red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::get_node_size() const
{
    return sizeof(red_black_node);
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
typename red_black_tree<tkey, tvalue, tkey_comparer>::color_node red_black_tree<tkey, tvalue, tkey_comparer>::get_color(
        red_black_node *current_node)
{
    return current_node == nullptr ? color_node::BLACK : current_node->color;
}

//end region help functions


//begin region insertion template method

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::after_insert_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive)
{
    if (path_to_subtree_root_exclusive.empty())
    {
        reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

        return;
    }
    else
    {
        if (subtree_root_address->left_subtree == nullptr &&
            subtree_root_address->right_subtree == nullptr)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
        }
    }

    red_black_node **parent = nullptr;
    red_black_node **uncle = nullptr;
    red_black_node **grand_parent = nullptr;
    red_black_node **great_grand_parent = nullptr;

    if (!path_to_subtree_root_exclusive.empty())
    {
        parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
        path_to_subtree_root_exclusive.pop();

        if (!path_to_subtree_root_exclusive.empty())
        {
            grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
            path_to_subtree_root_exclusive.pop();

            if ((*grand_parent)->left_subtree == *parent)
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->right_subtree));
            }
            else
            {
                uncle = reinterpret_cast<red_black_node**>(&((*grand_parent)->left_subtree));
            }

            if (!path_to_subtree_root_exclusive.empty())
            {
                great_grand_parent = reinterpret_cast<red_black_node**>(path_to_subtree_root_exclusive.top());
                path_to_subtree_root_exclusive.pop();
            }
        }
    }

    if (parent != nullptr &&
        _tree->get_color(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
    {
        if (great_grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(great_grand_parent));
        }

        if (grand_parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent));
        }

        path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(parent));

        return;
    }
    else
    {
        if (parent != nullptr &&
            grand_parent != nullptr &&
            _tree->get_color(*parent) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
        {
            if ((*parent)->right_subtree == subtree_root_address &&
                *parent == (*grand_parent)->left_subtree)
            {
                _tree->left_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent));
                subtree_root_address = (*parent)->left_subtree;
            }
            else
            {
                if (subtree_root_address == (*parent)->left_subtree &&
                    *parent == (*grand_parent)->right_subtree)
                {
                    _tree->right_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent));
                    subtree_root_address = (*parent)->right_subtree;
                }
            }

            if (_tree->get_color(*uncle) == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
            {
                (*parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;

                if (subtree_root_address == (*parent)->left_subtree &&
                    *parent == (*grand_parent)->left_subtree)
                {
                    _tree->right_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(great_grand_parent));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->right_subtree);
                }
                else
                {
                    _tree->left_rotate(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent), reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(great_grand_parent));
                    *parent = reinterpret_cast<red_black_node*>((*grand_parent)->left_subtree);
                }

                if (great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(great_grand_parent));
                }

                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent));
            }
            else
            {
                if (great_grand_parent != nullptr)
                {
                    path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(great_grand_parent));
                }

                (*uncle)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                (*grand_parent)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                auto **bin_grand_parent = reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(grand_parent);

                after_insert_inner(key, *bin_grand_parent, path_to_subtree_root_exclusive);

                path_to_subtree_root_exclusive.push(bin_grand_parent);
                path_to_subtree_root_exclusive.push(reinterpret_cast<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**>(parent));
            }
        }
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_insertion_template_method::red_black_insertion_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer> *tree) :
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
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::get_info_deleted_node(
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *deleted_node,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node**> &path_to_subtree_root_exclusive)
{
    _information_deleted_node = std::make_unique<red_black_node>();

    if (path_to_subtree_root_exclusive.empty())
    {
        _information_deleted_node->is_left_child = false;
        _information_deleted_node->is_right_child = false;
    }
    else
    {
        if ((*path_to_subtree_root_exclusive.top())->right_subtree == deleted_node)
        {
            _information_deleted_node->is_left_child = false;
            _information_deleted_node->is_right_child = true;
        }

        if ((*path_to_subtree_root_exclusive.top())->left_subtree == deleted_node)
        {
            _information_deleted_node->is_left_child = true;
            _information_deleted_node->is_right_child = false;
        }
    }

    _information_deleted_node->key = deleted_node->key;
    _information_deleted_node->value = deleted_node->value;
    _information_deleted_node->left_subtree = deleted_node->left_subtree;
    _information_deleted_node->right_subtree = deleted_node->right_subtree;
    _information_deleted_node->color = reinterpret_cast<red_black_node*>(deleted_node)->color;
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::after_remove_inner(
        tkey const &key,
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node *&subtree_root_address,
        std::stack<typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node **> &path_to_subtree_root_exclusive)
{
    if (subtree_root_address == nullptr)
    {
        return;
    }

    if ((_information_deleted_node->color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
        _information_deleted_node->color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK) &&
        (_information_deleted_node->left_subtree != nullptr &&
        _information_deleted_node->right_subtree != nullptr))
    {
        return;
    }

    if (_information_deleted_node->color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
        ((_information_deleted_node->left_subtree == nullptr && _information_deleted_node->right_subtree != nullptr) ||
        (_information_deleted_node->left_subtree != nullptr && _information_deleted_node->right_subtree == nullptr)))
    {
        if (_information_deleted_node->is_right_child == true)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        if (_information_deleted_node->is_left_child == true)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        if (_information_deleted_node->is_left_child == false &&
            _information_deleted_node->is_right_child == false)
        {
            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
        }

        return;
    }

    if (_information_deleted_node->color == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
        _information_deleted_node->left_subtree == nullptr && _information_deleted_node->right_subtree == nullptr)
    {
        typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_node **parent = nullptr;

        if (!path_to_subtree_root_exclusive.empty())
        {
            parent = path_to_subtree_root_exclusive.top();
            path_to_subtree_root_exclusive.pop();
        }

        if (_information_deleted_node->is_left_child == true)
        {
            if (subtree_root_address->right_subtree != nullptr)
            {
                color_node color_subtree = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address));
                color_node color_sibling = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree));
                color_node color_children_right = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree->right_subtree));
                color_node color_children_left = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree->left_subtree));

                if (color_sibling == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                {
                    if (color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
                        color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                    {
                        if (color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->left_rotate(&subtree_root_address, parent);
                        }

                        if (color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED &&
                            color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree->left_subtree)->color = color_sibling;
                            _tree->right_rotate(&(subtree_root_address->right_subtree), &subtree_root_address);

                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->left_rotate(&subtree_root_address, parent);
                        }
                    }

                    if (color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
                        color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                    {
                        reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                        reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

                        if (color_subtree == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            _information_deleted_node->color = reinterpret_cast<red_black_node*>(subtree_root_address)->color;
                            _information_deleted_node->key = subtree_root_address->key;
                            _information_deleted_node->value = subtree_root_address->value;
                            _information_deleted_node->left_subtree = subtree_root_address->left_subtree;
                            _information_deleted_node->right_subtree = subtree_root_address->right_subtree;

                            if (parent != nullptr)
                            {
                                if ((*parent)->left_subtree == subtree_root_address)
                                {
                                    _information_deleted_node->is_left_child = true;
                                    _information_deleted_node->is_right_child = false;
                                }
                                if ((*parent)->right_subtree == subtree_root_address)
                                {
                                    _information_deleted_node->is_left_child = false;
                                    _information_deleted_node->is_right_child = true;
                                }

                                after_remove_inner(key, *parent, path_to_subtree_root_exclusive);
                            }
                            else
                            {
                                path_to_subtree_root_exclusive.push(parent);
                                after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
                                path_to_subtree_root_exclusive.pop();
                            }
                        }
                    }
                }
                else
                {
                    reinterpret_cast<red_black_node*>(subtree_root_address->right_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                    reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                    _tree->left_rotate(&subtree_root_address, parent);

                    path_to_subtree_root_exclusive.push(parent);
                    path_to_subtree_root_exclusive.push(&subtree_root_address);
                    after_remove_inner(key, subtree_root_address->left_subtree, path_to_subtree_root_exclusive);
                    path_to_subtree_root_exclusive.pop();
                    path_to_subtree_root_exclusive.pop();
                }
            }
        }
        else
        {
            if (subtree_root_address->left_subtree != nullptr)
            {
                color_node color_subtree = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address));
                color_node color_sibling = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree));
                color_node color_children_right = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree->right_subtree));
                color_node color_children_left = _tree->get_color(reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree->left_subtree));

                if (color_sibling == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                {
                    if (color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED ||
                        color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                    {
                        if (color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->right_rotate(&subtree_root_address, parent);
                        }

                        if (color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED &&
                            color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree->right_subtree)->color = color_sibling;
                            _tree->left_rotate(&(subtree_root_address->left_subtree), &subtree_root_address);

                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = color_subtree;
                            reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                            _tree->right_rotate(&subtree_root_address, parent);
                        }
                    }

                    if (color_children_left == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK &&
                        color_children_right == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                    {
                        reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                        reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;

                        if (color_subtree == red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK)
                        {
                            _information_deleted_node->color = reinterpret_cast<red_black_node*>(subtree_root_address)->color;
                            _information_deleted_node->key = subtree_root_address->key;
                            _information_deleted_node->value = subtree_root_address->value;
                            _information_deleted_node->left_subtree = subtree_root_address->left_subtree;
                            _information_deleted_node->right_subtree = subtree_root_address->right_subtree;

                            if (parent != nullptr)
                            {
                                if ((*parent)->left_subtree == subtree_root_address)
                                {
                                    _information_deleted_node->is_left_child = true;
                                    _information_deleted_node->is_right_child = false;
                                }
                                if ((*parent)->right_subtree == subtree_root_address)
                                {
                                    _information_deleted_node->is_left_child = false;
                                    _information_deleted_node->is_right_child = true;
                                }

                                after_remove_inner(key, *parent, path_to_subtree_root_exclusive);
                            }
                            else
                            {
                                path_to_subtree_root_exclusive.push(parent);
                                after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
                                path_to_subtree_root_exclusive.pop();
                            }
                        }
                    }
                }
                else
                {
                    reinterpret_cast<red_black_node*>(subtree_root_address->left_subtree)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::BLACK;
                    reinterpret_cast<red_black_node*>(subtree_root_address)->color = red_black_tree<tkey, tvalue, tkey_comparer>::color_node::RED;
                    _tree->right_rotate(&subtree_root_address, parent);

                    path_to_subtree_root_exclusive.push(parent);
                    path_to_subtree_root_exclusive.push(&subtree_root_address);
                    after_remove_inner(key, subtree_root_address->right_subtree, path_to_subtree_root_exclusive);
                    path_to_subtree_root_exclusive.pop();
                    path_to_subtree_root_exclusive.pop();
                }
            }
        }

        if (parent != nullptr)
        {
            path_to_subtree_root_exclusive.push(parent);
        }
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::red_black_removing_template_method(
        red_black_tree<tkey, tvalue, tkey_comparer> *tree) :
        _tree(tree),
        binary_search_tree<tkey, tvalue, tkey_comparer>::bin_removing_template_method(tree)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_removing_template_method::~red_black_removing_template_method()
{

}

//end region removing template method


//begin region constructor

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(
        memory *allocator_binary_search_tree,
        logger *logger_tree) :
        binary_search_tree<tkey, tvalue, tkey_comparer>(
                new typename binary_search_tree<tkey, tvalue, tkey_comparer>::bin_find_template_method(this),
                new red_black_insertion_template_method(this),
                new red_black_removing_template_method(this),
                allocator_binary_search_tree,
                logger_tree)
{

}

//end region constructor


//begin region copy/move semantic

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(
        const red_black_tree<tkey, tvalue, tkey_comparer> &target_copy)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer> & red_black_tree<tkey, tvalue, tkey_comparer>::operator=(
        const red_black_tree<tkey, tvalue, tkey_comparer> &target_copy)
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>::red_black_tree(
        red_black_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept
{

}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
red_black_tree<tkey, tvalue, tkey_comparer>& red_black_tree<tkey, tvalue, tkey_comparer>::operator=(
        red_black_tree<tkey, tvalue, tkey_comparer> &&target_copy) noexcept
{

}

//end region copy/move semantic


#endif // RED_BLACK_TREE_H