#pragma once
#include <string>

class IField
{
public:
	IField() = default;
	virtual ~IField() = default;
	IField(const IField& other) = default;
	IField& operator=(const IField& other) = default;

	virtual void PrintValue() = 0;
	virtual std::string GetValue() = 0;
};
