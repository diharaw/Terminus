#include <io/input_map.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

InputMap::InputMap()
{
    for(auto& e : m_action_map.keyboard)
        e = UINT64_MAX;
    
    for(auto& e : m_action_map.mouse)
        e = UINT64_MAX;
    
    for(auto& e : m_action_map.gamepad)
        e = UINT64_MAX;
    
    for(auto& e : m_state_map.keyboard)
        e = UINT64_MAX;
    
    for(auto& e : m_state_map.mouse)
        e = UINT64_MAX;
    
    for(auto& e : m_state_map.gamepad)
        e = UINT64_MAX;
    
    for(auto& e : m_axis_map.keyboard_pos)
        e = UINT64_MAX;
    
    for(auto& e : m_axis_map.keyboard_neg)
        e = UINT64_MAX;
    
    for(auto& e : m_axis_map.mouse)
        e = UINT64_MAX;
    
    for(auto& e : m_axis_map.gamepad)
        e = UINT64_MAX;
}

InputMap::~InputMap()
{
    
}

StringHash InputMap::name()
{
    return m_name;
}

uint32_t InputMap::num_states()
{
    return m_state_map.count;
}

uint32_t InputMap::num_axis()
{
    return m_axis_map.count;
}

uint32_t InputMap::num_actions()
{
    return m_action_map.count;
}

void InputMap::set_action(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad)
{
    if(mouse != -1)
        m_action_map.mouse[mouse] = hash;
	else
	{
		for (auto& input_hash : m_action_map.mouse)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
 
    if(keyboard != -1)
        m_action_map.keyboard[keyboard] = hash;
	else
	{
		for (auto& input_hash : m_action_map.keyboard)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(gamepad != -1)
        m_action_map.gamepad[gamepad] = hash;
	else
	{
		for (auto& input_hash : m_action_map.gamepad)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
}

void InputMap::set_state(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad)
{
    if(mouse != -1)
        m_state_map.mouse[mouse] = hash;
	else
	{
		for (auto& input_hash : m_state_map.mouse)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(keyboard != -1)
        m_state_map.keyboard[keyboard] = hash;
	else
	{
		for (auto& input_hash : m_state_map.keyboard)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(gamepad != -1)
        m_state_map.gamepad[gamepad] = hash;
	else
	{
		for (auto& input_hash : m_state_map.gamepad)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
}

void InputMap::set_axis(uint64_t hash, int32_t mouse, int32_t keyboard_pos, int32_t keyboard_neg, int32_t gamepad)
{
    if(mouse != -1)
        m_axis_map.mouse[mouse - 9] = hash;
	else
	{
		for (auto& input_hash : m_axis_map.mouse)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(keyboard_pos != -1)
        m_axis_map.keyboard_pos[keyboard_pos] = hash;
	else
	{
		for (auto& input_hash : m_axis_map.keyboard_pos)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(keyboard_neg != -1)
        m_axis_map.keyboard_neg[keyboard_neg] = hash;
	else
	{
		for (auto& input_hash : m_axis_map.keyboard_neg)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
    
    if(gamepad != -1)
        m_axis_map.gamepad[gamepad] = hash;
	else
	{
		for (auto& input_hash : m_axis_map.gamepad)
		{
			if (input_hash == hash)
				input_hash = UINT64_MAX;
		}
	}
}

void InputMap::clear_action(StringHash hash)
{
    for(auto& h : m_action_map.keyboard)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_action_map.mouse)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_action_map.gamepad)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
}

void InputMap::clear_state(StringHash hash)
{
    for(auto& h : m_state_map.keyboard)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_state_map.mouse)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_state_map.gamepad)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
}

void InputMap::clear_axis(StringHash hash)
{
    for(auto& h : m_axis_map.keyboard_pos)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_axis_map.keyboard_neg)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_state_map.mouse)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
    
    for(auto& h : m_state_map.gamepad)
    {
        if(hash == h)
        {
            h = UINT64_MAX;
            break;
        }
    }
}

TE_END_TERMINUS_NAMESPACE
