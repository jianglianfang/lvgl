#if LV_BUILD_TEST
#include "../lvgl.h"
#include "../../lvgl_private.h"

#include "unity/unity.h"

static lv_obj_t * active_screen = NULL;
static lv_obj_t * textarea = NULL;

static const char * textarea_default_text = "";

void setUp(void)
{
    active_screen = lv_screen_active();
    textarea = lv_textarea_create(active_screen);
}

void tearDown(void)
{
    lv_obj_clean(active_screen);
}

void test_textarea_should_have_valid_documented_default_values(void)
{
    TEST_ASSERT(lv_textarea_get_cursor_click_pos(textarea));
    TEST_ASSERT_EQUAL(0U, lv_textarea_get_one_line(textarea));
    /* No placeholder text should be set on widget creation */
    TEST_ASSERT_EQUAL_STRING(textarea_default_text, lv_textarea_get_placeholder_text(textarea));
    TEST_ASSERT_EQUAL_STRING(textarea_default_text, lv_textarea_get_text(textarea));
}

/* When in password mode the lv_textarea_get_text function returns
 * the actual text, not the bullet characters. */
void test_textarea_should_return_actual_text_when_password_mode_is_enabled(void)
{
    const char * text = "Hello LVGL!";

    lv_textarea_add_text(textarea, text);
    lv_textarea_set_password_mode(textarea, true);

    TEST_ASSERT_TRUE(lv_textarea_get_password_mode(textarea));
    TEST_ASSERT_EQUAL_STRING(text, lv_textarea_get_text(textarea));
}

void test_textarea_should_update_label_style_with_one_line_enabled(void)
{
    lv_textarea_t * txt_ptr = (lv_textarea_t *) textarea;

    lv_textarea_add_text(textarea, "Hi");
    lv_textarea_set_one_line(textarea, true);

    int32_t left_padding = lv_obj_get_style_pad_left(txt_ptr->label, LV_PART_MAIN);
    int32_t right_padding = lv_obj_get_style_pad_right(txt_ptr->label, LV_PART_MAIN);
    int32_t line_width = lv_obj_get_width(txt_ptr->label);
    int32_t expected_size = left_padding + right_padding + line_width;

    TEST_ASSERT(lv_textarea_get_one_line(textarea));
    TEST_ASSERT_EQUAL_UINT16(expected_size, lv_obj_get_width(txt_ptr->label));
    TEST_ASSERT_EQUAL_UINT16(lv_pct(100), lv_obj_get_style_min_width(txt_ptr->label, LV_PART_MAIN));
}

void test_textarea_cursor_click_pos_field_update(void)
{
    lv_textarea_set_cursor_click_pos(textarea, false);

    TEST_ASSERT_FALSE(lv_textarea_get_cursor_click_pos(textarea));
}

void test_textarea_should_update_placeholder_text(void)
{
    const char * new_placeholder = "LVGL Rocks!!!!!";
    const char * text = "Hello LVGL!";

    /* Allocating memory for placeholder text */
    lv_textarea_set_placeholder_text(textarea, text);
    TEST_ASSERT_EQUAL_STRING(text, lv_textarea_get_placeholder_text(textarea));

    /* Reallocating memory for the new placeholder text */
    lv_textarea_set_placeholder_text(textarea, new_placeholder);
    TEST_ASSERT_EQUAL_STRING(new_placeholder, lv_textarea_get_placeholder_text(textarea));

    /* Freeing allocated memory for placeholder text */
    lv_textarea_set_placeholder_text(textarea, "");
    TEST_ASSERT_EQUAL_STRING("", lv_textarea_get_placeholder_text(textarea));
}

void test_textarea_should_keep_only_accepted_chars(void)
{
    const char * accepted_list = "abcd";

    lv_textarea_set_accepted_chars(textarea, accepted_list);
    lv_textarea_set_text(textarea, "abcde");

    TEST_ASSERT_EQUAL_STRING(accepted_list, lv_textarea_get_text(textarea));
}

void test_textarea_in_one_line_mode_should_ignore_line_break_characters(void)
{
    lv_textarea_set_one_line(textarea, true);

    lv_textarea_add_char(textarea, '\n');
    TEST_ASSERT_EQUAL_STRING(textarea_default_text, lv_textarea_get_text(textarea));

    lv_textarea_add_char(textarea, '\r');
    TEST_ASSERT_EQUAL_STRING(textarea_default_text, lv_textarea_get_text(textarea));
}

void test_textarea_should_hide_password_characters(void)
{
    lv_textarea_set_password_mode(textarea, true);
    lv_textarea_set_text(textarea, "12345");

    /* setting bullet hides characters */
    lv_textarea_set_password_bullet(textarea, "O");
    TEST_ASSERT_EQUAL_STRING("OOOOO", lv_label_get_text(lv_textarea_get_label(textarea)));

    /* setting text hides characters */
    lv_textarea_set_text(textarea, "A");
    TEST_ASSERT_EQUAL_STRING("O", lv_label_get_text(lv_textarea_get_label(textarea)));

    lv_textarea_add_char(textarea, 'B');
    TEST_ASSERT_EQUAL_STRING("OB", lv_label_get_text(lv_textarea_get_label(textarea)));

    /* setting show time hides characters */
    /* current behavior is to hide the characters upon setting the show time regardless of the value */
    lv_textarea_set_password_show_time(textarea, lv_textarea_get_password_show_time(textarea));
    TEST_ASSERT_EQUAL_STRING("OO", lv_label_get_text(lv_textarea_get_label(textarea)));

    lv_textarea_set_password_mode(textarea, false);
    TEST_ASSERT_EQUAL_STRING("AB", lv_label_get_text(lv_textarea_get_label(textarea)));

    /* enabling password mode hides characters */
    lv_textarea_set_password_mode(textarea, true);
    TEST_ASSERT_EQUAL_STRING("OO", lv_label_get_text(lv_textarea_get_label(textarea)));
}

#endif
